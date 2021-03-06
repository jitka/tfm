#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

class Tab;

class PartMassage: public QObject{
	Q_OBJECT
	public:
		int position; 
		QString name;
		int origTime; //v minutach
		int time;     //v sekundach
		bool chosen;
		bool origChosen;
		Tab* parent;

		QCheckBox* checkBox;
		QSpinBox* spinBox;
		QSlider* slider;
		QLabel* min;
		QLabel* sec;
	
	private slots:
		void onChange(int newValue);
		void onCheck(int state);
};


class Tab: public QWidget{
	Q_OBJECT
	public:

		Tab(QWidget* parent, QString name, QString path);
		void sliderChange(int part, int newValue);
	
	private slots:
		void onOk();
		void onReset();
		void onSumChange(int newValue);

	private:
		QVector<PartMassage*> parts;
		QSpinBox* totalTimeSpinBox;
		int sum;
		int origSum;
		
		int parse(QString name);
		void changeTimeTable();


};

#endif
