#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "timer.h"
#define TIME 10 //zrychlene
//#define TIME 1000 //realne
Timer::Timer(QVector<part_massage> parts) {

	setWindowState(Qt::WindowFullScreen);
	setStyleSheet("background-image: url(./background.jpg); background-position: center");

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(30,100,30,100); //TODO proc je v pravo vetsi dira?
	vbox->addStretch();
/*
struct part_massage{
	QString name;
	int time;
	int final_time;
	bool chosen;
	bool final_chosen;
	QProgressBar* progresBar;
*/
	
	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->setContentsMargins(0,0,0,0);
	vbox->addLayout(hbox);
	
	prvni = new QProgressBar(this);
	prvni->setMinimum(0);
	prvni->setMaximum(60);
	prvni->setValue(0);
	prvni->setFormat("pokus");
	prvni->setTextVisible(true);
	hbox->addWidget(prvni);

	QProgressBar *druha = new QProgressBar(this);
	hbox->addWidget(druha);

	for (int i = 0; i < parts.size(); i++){
		if (!parts[i].final_chosen)
			continue;
	}

	show();
	startTimer(TIME);

}

void Timer::keyPressEvent(QKeyEvent *event){
	event = 0;

	//TODO neco jako this->close();
		          qApp->quit();
}

void Timer::timerEvent(QTimerEvent *event){
	event = 0;
	prvni->setValue((prvni->value())+1);
}

