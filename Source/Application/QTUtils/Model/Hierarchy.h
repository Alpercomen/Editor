#pragma once
#pragma once
#include <QStandardItemModel> // Base class

class HierarchyModel : public QStandardItemModel {
public:
	explicit HierarchyModel(QObject* parent = nullptr);
};