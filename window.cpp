#include <iostream>
#include <QIcon>
#include <QVBoxLayout>
#include <QDir>
#include <QMessageBox>
#include "window.h"
#include "tab.h"

#define W_WIDTH 500
#define W_HEIGHT 400


Window::Window(){

	resize(W_WIDTH, W_HEIGHT);
	setWindowTitle("Timer for massage");
	setWindowIcon(QIcon("icon.jpg"));

	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->setContentsMargins(0,0,0,0);

	tabs = new QTabWidget(this);
	tabs->setTabsClosable(true);
	tabs->setMovable(true);
	connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(onCloseTab(int)));
	vbox->addWidget(tabs);

	QDir dir = QDir::home();
	if (!dir.cd(".tfm")){
		QMessageBox::critical (this,"Error", "Setting not found");
		show();

	} else {

		QFileInfoList list = dir.entryInfoList();

		for (int i = 0; i < list.size(); ++i) {
			if (list[i].isFile()){
				Tab *tab = new Tab(this,list[i].fileName(),list[i].canonicalFilePath());
				tabs->addTab (tab,list[i].fileName());
			}
		} 
		show();

	}
}

void Window::onCloseTab(int index){
	tabs->removeTab(index);
}
