#pragma once
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>
#include <QDir>

#include <Application/QTUtils/View/ContentBrowser.h>

ContentBrowser::ContentBrowser(QWidget* parent) : QWidget(parent) {
	auto* layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	auto* hl = new QHBoxLayout();
	search = new QLineEdit(this);
	search->setPlaceholderText("Filter (*.png;*.json; name substring)");
	hl->addWidget(new QLabel("Filter:"));
	hl->addWidget(search, 1);
	layout->addLayout(hl);


	model = new QFileSystemModel(this);
	model->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files);
	const QString root = QDir::currentPath();
	model->setRootPath(root);


	proxy = new QSortFilterProxyModel(this);
	proxy->setSourceModel(model);
	proxy->setRecursiveFilteringEnabled(true);
	proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);


	view = new QListView(this);
	view->setModel(proxy);
	view->setViewMode(QListView::IconMode);
	view->setResizeMode(QListView::Adjust);
	view->setIconSize(QSize(32, 32));
	view->setSelectionMode(QAbstractItemView::SingleSelection);
	view->setRootIndex(proxy->mapFromSource(model->index(root)));


	layout->addWidget(view, 1);


	connect(search, &QLineEdit::textChanged, this, [this](const QString& t) {
		proxy->setFilterWildcard(t);
		});
}


void ContentBrowser::setRootPath(const QString& path) {
	model->setRootPath(path);
	view->setRootIndex(proxy->mapFromSource(model->index(path)));
}