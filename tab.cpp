#include <QPushButton>
#include "tab.h"

Tab::Tab(QWidget *parent, QString name):QWidget(parent){
	
	setToolTip(name);

	QPushButton *quit = new QPushButton("Quit", this);
   	quit->setGeometry(50, 40, 75, 30);


}
