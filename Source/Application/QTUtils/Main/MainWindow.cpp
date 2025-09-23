#pragma once
#include <QStyle>
#include <QTreeView>
#include <QStatusBar>
#include <QModelIndex>
#include <QToolbar>
#include <QDockwidget>

#include <Application/QTUtils/Main/MainWindow.h>
#include <Application/QTUtils/Model/Hierarchy.h>
#include <Application/QTUtils/Panel/Inspector.h>
#include <Application/QTUtils/View/ContentBrowser.h>
#include <Application/QTUtils/View/Scene.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setWindowTitle("Nyx Editor – Skeleton");
	resize(1400, 900);


	// Central scene view
	sceneView = new SceneView(this);
	setCentralWidget(sceneView);


	buildMenusAndToolbars();
	buildDocks();


	statusBar()->showMessage("Ready");
}


void MainWindow::buildMenusAndToolbars() 
{
	auto* tb = addToolBar("Playback");
	actPlay = tb->addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
	actStop = tb->addAction(style()->standardIcon(QStyle::SP_MediaStop), "Stop");
	actFF = tb->addAction(style()->standardIcon(QStyle::SP_MediaSkipForward), "Fast-Forward");
	tb->setMovable(false);


	connect(actPlay, &QAction::triggered, this, [this] { statusBar()->showMessage("Play"); });
	connect(actStop, &QAction::triggered, this, [this] { statusBar()->showMessage("Stop"); });
	connect(actFF, &QAction::triggered, this, [this] { statusBar()->showMessage("Fast-Forward"); });
}

void MainWindow::buildDocks() {
	// Hierarchy Dock (Left)
	auto* hierarchyDock = new QDockWidget("Hierarchy", this);
	hierarchyDock->setObjectName("HierarchyDock");
	hierarchyModel = new HierarchyModel(this);
	auto* hierarchyView = new QTreeView(hierarchyDock);
	hierarchyView->setModel(hierarchyModel);
	hierarchyView->setHeaderHidden(false);
	hierarchyView->expandAll();
	hierarchyDock->setWidget(hierarchyView);
	addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);


	// Inspector Dock (Right)
	auto* inspectorDock = new QDockWidget("Inspector", this);
	inspectorDock->setObjectName("InspectorDock");
	inspectorPanel = new InspectorPanel(inspectorDock);
	inspectorDock->setWidget(inspectorPanel);
	addDockWidget(Qt::RightDockWidgetArea, inspectorDock);


	// Content Browser Dock (Bottom)
	auto* contentDock = new QDockWidget("Content Browser", this);
	contentDock->setObjectName("ContentDock");
	contentBrowser = new ContentBrowser(contentDock);
	contentDock->setWidget(contentBrowser);
	addDockWidget(Qt::BottomDockWidgetArea, contentDock);


	// Selection handling: update Inspector when Hierarchy selection changes
	connect(hierarchyView->selectionModel(), &QItemSelectionModel::currentChanged,
		this, [this](const QModelIndex& curr, const QModelIndex&) {
			const QString name = hierarchyModel->data(curr, Qt::DisplayRole).toString();
			inspectorPanel->showEntity(name);
		});
}