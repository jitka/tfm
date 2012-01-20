#include <QApplication>
#include "window.h"
#include "timer.h"

#define W_WIDTH 250
#define W_HEIGHT 150


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

//	Window window;
	Timer timer;

	return app.exec();
}


