#pragma once
#include <QWidget> // Base class

class SceneView : public QWidget {
public:
	explicit SceneView(QWidget* parent = nullptr);
protected:
	void paintEvent(QPaintEvent*) override;
};