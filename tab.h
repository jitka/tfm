#ifndef TAB_H
#define TAB_H
#include <QWidget>
#include <QVector>

struct part_massage{
	QString name;
	int time;
	bool common;
};

class Tab: public QWidget{
	public:
		Tab(QWidget *parent, QString name);
	private:
		QVector<part_massage> parts;
		int parse(QString name);


};

#endif
