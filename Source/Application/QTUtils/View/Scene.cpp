#include <Application/QTUtils/View/Scene.h>
#include <QPainter>
#include <QColor>

SceneView::SceneView(QWidget* parent) : QWidget(parent) {
	setAutoFillBackground(false);
	setFocusPolicy(Qt::StrongFocus);
	setMinimumSize(640, 360);
}

void SceneView::paintEvent(QPaintEvent*) {
	QPainter p(this);
	p.fillRect(rect(), Qt::black);
	p.setPen(QColor(60, 60, 60));
	const int step = 32;
	for (int x = 0; x < width(); x += step) p.drawLine(x, 0, x, height());
	for (int y = 0; y < height(); y += step) p.drawLine(0, y, width(), y);
	p.setPen(Qt::white);
	p.drawText(12, 20, "Scene View (Nyx integration point)");
}