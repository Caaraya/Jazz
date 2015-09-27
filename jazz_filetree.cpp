#include "jazz_filetree.hpp"
#include <giomm.h>

namespace Jazz
{
	FileTree::FileTree(const std::string& path):tree_store(),
		cancellation_token(Gio::Cancellable::create())
	{
		add(tree_view);
		// create tree model
		tree_view.append_column("Name", column.filename);
		
		tree_store = Gtk::TreeStore::create(column);
		tree_view.set_model(tree_store);
		
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
		
		file->enumerate_children_async(
			[this, file](Glib::RefPtr<Gio::AsyncResult>& result) -> void
			{
				puts("Async result has happened");
				Glib::RefPtr<Gio::FileEnumerator> enumer = file->enumerate_children_finish(result);
				
				enumer->next_files_async(
					[this, file, enumer](Glib::RefPtr<Gio::AsyncResult>& res)-> void
					{
						Glib::ListHandle<Glib::RefPtr<Gio::FileInfo>> files =
							enumer->next_files_finish(res);
							
							Gtk::TreeModel::Row row;
							//Gtk::TreeModel::Row childrow = *(tree_store->append(row.children));
							
						puts("Another async Uhhhh : )");
						for(auto the_file : files)
						{
							if (the_file->get_file_type() == Gio::FileType::FILE_TYPE_DIRECTORY)
							{
								puts(the_file->get_name().c_str());
								puts(" is a director");
							}
							row = *(tree_store->append());
							row[column.filename] = the_file->get_name();
							puts(the_file->get_name().c_str());
						}
						show_all_children();
					},100);
			},
			cancellation_token,
			"standard::*");
			
			
				
		/*
		// Fill treeView
		Gtk::TreeModel::Row row = *(tree_store->append());
  	row[column.filename] = "Billy Bob";
  	
  	Gtk::TreeModel::Row childrow = *(tree_store->append(row.children()));
  	childrow = *(tree_store->append(row.children()));
		childrow[column.filename] = "Sue Bob";

		row = *(tree_store->append());
		row[column.filename] = "Joey Jojo";


		row = *(tree_store->append());
		row[column.filename] = "Rob McRoberts";

		childrow = *(tree_store->append(row.children()));
		childrow[column.filename] = "Xavier McRoberts";

		//Add the TreeView's view columns:
		tree_view.append_column("Name", column.filename);

		add(tree_view);
		//Connect signal:
		//tree_view.signal_row_activated().connect(sigc::mem_fun(*this,
		//            &ExampleWindow::on_treeview_row_activated) );
  	
  	show_all_children();*/
	}
}
