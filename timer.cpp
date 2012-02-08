#include <QVBoxLayout>
#include <QHBoxLayout>
#include "timer.h"
//#define TIME 10 //zrychlene
#define TIME 1000 //realne



Timer::Timer(QVector<pbInfo> &v){

	setWindowState(Qt::WindowFullScreen);
	setStyleSheet("background-image: url(./background.jpg); background-position: center");

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(30,100,30,100); 
	vbox->addStretch();
	
	QHBoxLayout *hbox = new QHBoxLayout();
	hbox->setContentsMargins(0,0,0,0);
	hbox->setSpacing(0);
	vbox->addLayout(hbox);

	for (int i = 0; i < v.size(); i++){
		QProgressBar *pb = new QProgressBar(this);
		pb->setMinimum(0);
		pb->setMaximum(v[i].time);
		pb->setValue(0); //je potreba aby byl videt text
		pb->setFormat(v[i].name);
		pb->setTextVisible(true);
		pb->setContentsMargins(0,0,0,0);
		hbox->addWidget(pb,v[i].time);
		progressBars.append(pb);
	}
	
	current_part=0;
	show();
	timerId = startTimer(TIME);

}

void Timer::keyPressEvent(QKeyEvent *event){
	event = 0;
	this->close();
}

void Timer::timerEvent(QTimerEvent *event){
	//posouva progressBar
	if (progressBars[current_part]->value() >= progressBars[current_part]->maximum() ){
		current_part++;
		if (current_part >= progressBars.size()){
			killTimer(timerId);
			return;
		}
	}
	
	QProgressBar* pb = progressBars[current_part];
	pb->setValue(pb->value()+1);

}

