#ifndef TIMER_H_
#define TIMER_H_
#include <QWidget>
#include <QTimer>
#include <QProgressBar>

struct pbInfo{
	QString name;
	int time;
};


class Timer: public QWidget{
	public:
		Timer(QVector<pbInfo> &v);
	protected:
		void keyPressEvent(QKeyEvent* event);
		void timerEvent(QTimerEvent* event);
	private:
		QVector<QProgressBar*> progressBars;
		int current_part;
		int timerId;
};


#endif
