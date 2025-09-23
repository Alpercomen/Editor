#include <Application/QTUtils/Model/Hierarchy.h>

HierarchyModel::HierarchyModel(QObject* parent) : QStandardItemModel(parent) 
{
	setHorizontalHeaderLabels({ "Entity" });
	auto* root = invisibleRootItem();
	auto* e1 = new QStandardItem("Camera"); e1->setEditable(false);
	auto* e2 = new QStandardItem("DirectionalLight"); e2->setEditable(false);
	auto* e3 = new QStandardItem("Player"); e3->setEditable(false);
	auto* e3_child = new QStandardItem("Weapon"); e3_child->setEditable(false);
	e3->appendRow(e3_child);
	root->appendRow(e1);
	root->appendRow(e2);
	root->appendRow(e3);
}