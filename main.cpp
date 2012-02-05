#include <QApplication>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QFile>
#include "window.h"
//#include "json.h"
//#include "timer.h"

// je potreba mit v qt takovy vzhled kdy jsou procenta progresbaru uprostred

int main(int argc, char *argv[])
{
	//QFile file("json.txt");
/*	QFile file("/home/jitka/.tfm/aeiou");
	//QFile file("/home/jitka/.tfm/druhe");
//	QFile file("vstup.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return 1;

	QTextStream in(&file);
	QString in2 = in.readAll();

	Json p(in2);

//	qDebug() << "neco";
	QFile file2("/dev/stdout");
	if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
		return 1;
	QTextStream out(&file2);

	p.write(out);
*/
//	QString str;
//	in >> str;
//	qDebug() << str;

	QApplication app(argc, argv);

	Window window;
//	Timer timer;
	return app.exec();
}


