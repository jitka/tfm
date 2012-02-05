#ifndef TAB_H
#define TAB_H
#include <QWidget>
#include <QVector>
#include <QCheckBox>
#include <QSpinBox>
#include <QProgressBar>
//#include "part_massage.h"
struct part_massage{
	QString name;
	int time;
	int final_time;
	bool chosen;
	bool final_chosen;

	QCheckBox* checkBox;
	QSpinBox* spinBox;
	QProgressBar* progresBar;
};


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
