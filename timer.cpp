#include <QProgressBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "timer.h"

Timer::Timer(){
	setWindowState(Qt::WindowFullScreen);
	setStyleSheet("background-image: url(./background.jpg); background-position: center");

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(30,100,30,100); //TODO proc je v pravo vetsi dira?
	vbox->addStretch();

	QHBoxLayout *hbox = new QHBoxLayout();
//	hbox->setContentsMargins(0,0,0,0);
	vbox->addLayout(hbox);
	
	QProgressBar *prvni = new QProgressBar(this);
	hbox->addWidget(prvni);

	QProgressBar *druha = new QProgressBar(this);
	hbox->addWidget(druha);

	show();

}

void Timer::keyPressEvent(QKeyEvent *event){
	//TODO neco jako this->close();
	   if (event->key() == Qt::Key_Escape) {  
		          qApp->quit();
			     } 
}

