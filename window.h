#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QTabWidget>

class Window: public QWidget{
	Q_OBJECT
	public:
		Window();

	private slots:
		void onCloseTab(int index);
	private:
		QTabWidget *tabs;

};

#endif
