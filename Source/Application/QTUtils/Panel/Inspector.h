#pragma once
#include <QWidget> // Base class
#include <QTableWidget> // Uses QTableWidget directly


class InspectorPanel : public QWidget {
	Q_OBJECT
public:
	explicit InspectorPanel(QWidget* parent = nullptr);


public slots:
	void showEntity(const QString& name);


private:
	void addKV(const QString& k, const QString& v);
	QTableWidget* table{ nullptr };
};