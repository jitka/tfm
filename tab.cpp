#include <QPushButton>
#include <fstream>
#include <iostream>
#include <QString>
#include <QMessageBox>
#include <QLabel>
#include "tab.h"
#include "json.h"

Tab::Tab(QWidget *parent, QString name):QWidget(parent){
	
	if (parse(name)) {
	setToolTip(name);

	QPushButton *quit = new QPushButton("Nemackat", this);
   	quit->setGeometry(50, 40, 75, 30);

	} else {
		//nepovedlo se nacit konfigurak
		setToolTip(name);
		QLabel *label = new QLabel("Nepodarilo se naparsovat konfigurak.",this );
		QPushButton *quit = new QPushButton("Quit", this);
		quit->setGeometry(50, 40, 75, 30);
	}


}

int Tab::parse(QString name){

	name = "tralala";
//	std::ifstream input(name.toStdString());
//	polozka p = parsuj(input);		
//polozka parsuj(std::istream &vstup);
	return false;
}
