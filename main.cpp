#include <QApplication>
#include "window.h"

// je potreba mit v qt takovy vzhled kdy jsou procenta progresbaru uprostred

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Window window;
	
	return app.exec();
}


