#pragma once
#include <QMainWindow> // Base class

class SceneView; // fwd
class HierarchyModel; // fwd
class InspectorPanel; // fwd
class ContentBrowser; // fwd
class QAction; // fwd


class MainWindow : public QMainWindow {
public:
	explicit MainWindow(QWidget* parent = nullptr);

private:
	// helpers
	void buildMenusAndToolbars();
	void buildDocks();

	// Central view
	SceneView* sceneView{ nullptr };

	// Dock widgets content
	HierarchyModel* hierarchyModel{ nullptr };
	InspectorPanel* inspectorPanel{ nullptr };
	ContentBrowser* contentBrowser{ nullptr };

	// Actions
	QAction* actPlay{ nullptr };
	QAction* actStop{ nullptr };
	QAction* actFF{ nullptr };
};