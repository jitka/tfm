#ifndef TAB_H
#define TAB_H
#include <QWidget>
#include <QVector>
#include <QCheckBox>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>

struct part_massage{
	QString name;
	int time;
	bool chosen;

	QCheckBox* checkBox;
	QSpinBox* spinBox;
	QSlider* slider;
	QLabel* label;
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
