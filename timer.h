#ifndef TIMER_H_
#define TIMER_H_
#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include "part_massage.h"

class Timer: public QWidget{
	public:
		Timer(QVector<part_massage> parts);
	protected:
		void keyPressEvent(QKeyEvent* event);
		void timerEvent(QTimerEvent* event);
	private:
		QProgressBar* prvni;
};


#endif
