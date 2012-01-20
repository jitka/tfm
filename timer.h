#ifndef TIMER_H_
#define TIMER_H_
#include <QWidget>
#include <QProgressBar>
#include <QTimer>

class Timer: public QWidget{
	public:
		Timer();
	protected:
		void keyPressEvent(QKeyEvent * event);
		void timerEvent(QTimerEvent *event);
	private:
		QProgressBar* prvni;
};


#endif
