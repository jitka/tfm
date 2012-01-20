#include <QIcon>
#include <QTabWidget>
#include <QVBoxLayout>
#include "window.h"
#include "tab.h"

#define W_WIDTH 250
#define W_HEIGHT 150

Window::Window(){

	resize(W_WIDTH, W_HEIGHT);
	setWindowTitle("Timer for massage");
	setWindowIcon(QIcon("icon.jpg"));
	setToolTip("window");

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0,0,0,0);

	QTabWidget *tabs = new QTabWidget(this);
	vbox->addWidget(tabs);

	Tab *tab1 = new Tab(this,"prvni");
	tabs->addTab (tab1,"prvni");

	Tab *tab2 = new Tab(this,"druhy");
	tabs->addTab (tab2,"druhy");

	show();


}
