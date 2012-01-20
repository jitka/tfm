#include <QApplication>
#include <QWidget>
#include <QIcon>
#include <QTabWidget>

#define W_WIDTH 250
#define W_HEIGHT 150


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QWidget window;

	QTabWidget tabs(&window);
	QWidget inside;
	inside.setToolTip("inside");
	tabs.addTab (&inside,"prvni");

	window.resize(W_WIDTH, W_HEIGHT);
	window.setWindowTitle("Timer for massage");
	window.setWindowIcon(QIcon("icon.jpg"));
	window.setToolTip("window");
	window.show();

	return app.exec();
}


