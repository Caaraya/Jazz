#include "jazz_filetree.hpp"

namespace Jazz
{
	FileTree::FileTree(const std::string& path):tree_store()
	{
		//FileTreeModelColumns column;
		
		// create tree model
		tree_store = Gtk::TreeStore::create(column);
		tree_view.set_model(tree_store);
		
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
  	
  	show_all_children();
	}
}
