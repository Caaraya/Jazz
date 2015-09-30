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
		
		const Gtk::TreeModel::Row& row = *(tree_store->append());
  	row[column.filename] = path;
  	
  	DrawTree(path, row);
	}
	void FileTree::DrawTree(const std::string& path, const Gtk::TreeModel::Row& parent)
	{
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);
	
		file->enumerate_children_async(
		[this, file, parent, path](Glib::RefPtr<Gio::AsyncResult>& result) -> void
		{
			Glib::RefPtr<Gio::FileEnumerator> enumer = file->enumerate_children_finish(result);
			
			enumer->next_files_async(
				[this, file, parent, enumer, path](Glib::RefPtr<Gio::AsyncResult>& res)-> void
				{
					Glib::ListHandle<Glib::RefPtr<Gio::FileInfo>> files =	enumer->next_files_finish(res);

					for(auto the_file : files)
					{
						Gtk::TreeModel::Row childrow = *(tree_store->append(parent->children()));
						childrow[column.filename] = the_file->get_name();

						if (the_file->get_file_type() == Gio::FileType::FILE_TYPE_DIRECTORY)
						{
							DrawTree(path+"/"+the_file->get_name(), childrow);
						}
					}
					show_all_children();
				},100);
		},
		cancellation_token,
		"standard::*");
	}
}
