#ifndef TAB_H
#define TAB_H
#include <QWidget>
#include <QVector>
#include <QCheckBox>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

class Tab;

class PartMassage: public QObject{
	Q_OBJECT
	public:
		//TODO toto bude trida a bude chytat signal valueChanged() slideru
		int position; 
		QString name;
		int origTime;
		int time;
		bool chosen;
		Tab* parent;

		QCheckBox* checkBox;
		QSpinBox* spinBox;
		QSlider* slider;
		QLabel* label;
	
	private slots:
		void onChange(int newValue);
};


class Tab: public QWidget{
	Q_OBJECT
	public:

		Tab(QWidget* parent, QString name, QString path);
		void changeTimeTable(int part, int newValue);
	
	private slots:
		void onOk();

	private:
		QVector<PartMassage*> parts;
		int parse(QString name);
		QSpinBox* totalTimeSpinBox;


};

#endif
