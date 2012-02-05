#ifndef TAB_H
#define TAB_H
#include <QWidget>
#include <QVector>
#include "part_massage.h"

class Tab: public QWidget{
	Q_OBJECT
	public:
		Tab(QWidget* parent, QString name, QString path);
	
	private slots:
		void onOk();

	private:
		QVector<part_massage> parts;
		int parse(QString name);


};

#endif
