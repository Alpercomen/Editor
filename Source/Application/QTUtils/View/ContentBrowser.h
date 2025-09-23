#pragma once
#include <QWidget> // Base class

class QLineEdit; class QListView; class QFileSystemModel; class QSortFilterProxyModel;


class ContentBrowser : public QWidget {
public:
	explicit ContentBrowser(QWidget* parent = nullptr);
	void setRootPath(const QString& path);
private:
	QLineEdit* search{ nullptr };
	QFileSystemModel* model{ nullptr };
	QSortFilterProxyModel* proxy{ nullptr };
	QListView* view{ nullptr };
};