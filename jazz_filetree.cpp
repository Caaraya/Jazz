#include "jazz_filetree.hpp"
#include <giomm.h>
#include <iostream>

namespace Jazz
{
	FileTree::FileTree(const Glib::ustring& path):tree_store(),
		cancellation_token(Gio::Cancellable::create())
	{
		add(tree_view);
		// create tree model
		tree_view.append_column("Name", column.filename);
		
		tree_store = Gtk::TreeStore::create(column);
		tree_view.set_model(tree_store);

        // The tree view doesn't need to be cleared the first time
        SetPath(path);
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
					Glib::ListHandle<Glib::RefPtr<Gio::FileInfo>> files = enumer->next_files_finish(res);

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
    void FileTree::SetPath(const Glib::ustring& path, bool clear)
    {
        this->path = path;
        if(clear)
            tree_store->clear();
        
        //const Gtk::TreeModel::Row& row = *(tree_store->append());
		root = *(tree_store->append());
		
		// This deals with the case of being passed in current directory: ./ or prev directory: ../
		Glib::RefPtr<Gio::File> file = Gio::File::create_for_path(path);

		//row[column.filename] = file->get_basename();
		root[column.filename] = file->get_basename();
		
		//DrawTree(path, row);
		DrawTree(path, root);
    }
}
