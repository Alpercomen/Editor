#pragma once
#include <Application/Core/Core.h>
#include <Application/QTUtils/MainWindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}