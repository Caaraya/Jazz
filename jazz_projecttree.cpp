#include "jazz_projecttree.hpp"
#include <assert.h>
using namespace coral::zircon;
namespace Jazz
{
	ProjectTreeModelColumns::ProjectTreeModelColumns()
	{
		add(filename);
	}
	ProjectTreeView::ProjectTreeView() :
      cancellation_token(Gio::Cancellable::create())
	{
		add(tree_view);
      
      tree_view.append_column("Project", column.filename);
      
      tree_store = Gtk::TreeStore::create(column);
      tree_view.set_model(tree_store);
	}
   ProjectTreeView::ProjectTreeView(object& document) :
		ProjectTreeView()
   {      
		SetProject(document);
   }
	void ProjectTreeView::SetProject(object& document)
	{
		if(!document.has("project_name"))
			throw std::invalid_argument("Project lacking name");
			
		tree_store->clear();
		const Gtk::TreeModel::Row& project_row = *(tree_store->append());
		
		project_row[column.filename] = document["project_name"].as<string>();
		
		if(document.has("source_files"))
		{
			array source_files = document["source_files"].as<array>();
			
			for(auto file : source_files)
			{
				Gtk::TreeModel::Row source_file_row = *(tree_store->append(project_row->children()));
				source_file_row[column.filename] = file.as<string>();
			}
		}
	}
}