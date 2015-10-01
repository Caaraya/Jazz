#include "jazz_filetree.hpp"
#include <giomm.h>
#include <iostream>

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
		tree_view.signal_row_activated().connect(sigc::mem_fun(*this, &FileTree::OpenSelected) );
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
						if(!the_file->is_backup() && !the_file->is_hidden())
						{
							Gtk::TreeModel::Row childrow = *(tree_store->append(parent->children()));
							childrow[column.filename] = the_file->get_name();

							if (the_file->get_file_type() == Gio::FileType::FILE_TYPE_DIRECTORY)
							{
								DrawTree(path+"/"+the_file->get_name(), childrow);
							}
						}
					}
					show_all_children();
				},100);
		},
		cancellation_token,
		"standard::*");
	}
	void FileTree::OpenSelected(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn*)
	{
		Gtk::TreeModel::iterator iter = tree_store->get_iter(path);
		if(iter)
		{
			Gtk::TreeModel::Row row = *iter;
			std::cout << "Row activated: filename=" << row[column.filename] << std::endl;
		}
	}
}
