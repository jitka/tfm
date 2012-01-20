#ifndef TIMER_H_
#define TIMER_H_
#include <QWidget>

class Timer: public QWidget{
	  public:
		  Timer();
	  protected:
		      void keyPressEvent(QKeyEvent * event);

};


#endif
