#pragma once
#include <Application/QTUtils/Panel/Inspector.h>
#include <QVBoxLayout>
#include <QHeaderview>

InspectorPanel::InspectorPanel(QWidget* parent) : QWidget(parent) {
	table = new QTableWidget(this);
	table->setColumnCount(2);
	table->setHorizontalHeaderLabels({ "Property", "Value" });
	table->horizontalHeader()->setStretchLastSection(true);
	table->verticalHeader()->setVisible(false);
	table->setShowGrid(false);
	table->setSelectionMode(QAbstractItemView::SingleSelection);
	table->setSelectionBehavior(QAbstractItemView::SelectItems);


	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(table);
}


void InspectorPanel::showEntity(const QString& name) {
	table->setRowCount(0);
	addKV("Name", name);
	if (name == "Camera") {
		addKV("Transform/Position", "0, 1, 5");
		addKV("Camera/FOV", "60");
		addKV("Camera/NearFar", "0.1 / 1000");
	}
	else if (name == "DirectionalLight") {
		addKV("Intensity", "3.0");
		addKV("Color", "1.0, 0.95, 0.8");
	}
	else if (name == "Player") {
		addKV("HP", "100");
		addKV("Speed", "6.5");
	}
	else {
		addKV("Info", "(no components)");
	}
}


void InspectorPanel::addKV(const QString& k, const QString& v) {
	const int r = table->rowCount();
	table->insertRow(r);
	table->setItem(r, 0, new QTableWidgetItem(k));
	table->setItem(r, 1, new QTableWidgetItem(v));
}