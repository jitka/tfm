#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QDebug>

#include "timer.h"
#define TIME 100 //zrychlene
//#define TIME 1000 //realne
Timer::Timer(QVector<part_massage> &parts2) {

	setWindowState(Qt::WindowFullScreen);
	setStyleSheet("background-image: url(./background.jpg); background-position: center");

	this->parts = parts2;

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(30,100,30,100); //TODO proc je v pravo vetsi dira?
	vbox->addStretch();
	
	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->setContentsMargins(0,0,0,0);
	hbox->setSpacing(0);
	vbox->addLayout(hbox);

	for (int i = 0; i < parts.size(); i++){
		if ( parts[i].final_chosen == false)
			continue;
		parts[i].progresBar = new QProgressBar(this);
		parts[i].progresBar->setMinimum(0);
		parts[i].progresBar->setMaximum(parts[i].final_time*60);
		parts[i].progresBar->setValue(0); //je potreba aby byl videt text
		parts[i].progresBar->setFormat(parts[i].name);
		parts[i].progresBar->setTextVisible(true);
		parts[i].progresBar->setContentsMargins(0,0,0,0);
		hbox->addWidget(parts[i].progresBar,parts[i].final_time);
	}
	
	current_part=0;
	show();
	startTimer(TIME);

}

void Timer::keyPressEvent(QKeyEvent *event){
	event = 0;

	//TODO neco jako this->close();
		          qApp->quit();
}

void Timer::timerEvent(QTimerEvent *event){
	if (current_part >= parts.size())
		return;

	while ( parts[current_part].final_chosen == false ||
			parts[current_part].progresBar->value() >= parts[current_part].final_time*60 ){

		current_part++;
		if (current_part >= parts.size())
			return;
	}

	QProgressBar* pb = parts[current_part].progresBar;
	pb->setValue(pb->value()+1);

}

