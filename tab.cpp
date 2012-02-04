#include <QPushButton>
#include <fstream>
#include <iostream>
#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <QTextStream>
#include <QDebug>
#include <vector>
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
		//QLabel *label = new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		QPushButton *quit = new QPushButton("Quit", this);
		quit->setGeometry(50, 40, 75, 30);
	}


}

int Tab::parse(QString name){

	std::string n = "/home/jitka/.tfm/"+name.toStdString();
	//std::cout << n << "\n";
	std::ifstream input(n.c_str());
	polozka p = parsuj(input);		

	if (p.typ == VECTOR){
		std::vector<polozka> *v = p.hodnota.Vector;
		for (int i = 0; i < (int) v->size(); i++){
			if ((*v)[i].typ != MAP)
				goto AU;
			std::map<std::string,polozka> *m = (*v)[i].hodnota.Map;

			part_massage p;

			if ( (*m).find("name") == (*m).end())
				goto AU;
			if ( (*m)["name"].typ != STRING )
				goto AU;
			p.name = QString((*((*m)["name"].hodnota.String)).c_str());


			if ( (*m).find("time") == (*m).end())
				goto AU;
			if ( (*m)["time"].typ != INT )
				goto AU;
			p.time = (*m)["time"].hodnota.Int;

			if ( (*m).find("default") == (*m).end())
				goto AU;
			switch ( (*m)["default"].typ ){
			case MY_TRUE:
				p.common = true;
				break;
			case MY_FALSE:
				p.common = false;
				break;
			default:
				goto AU;
				break;
			}

			parts.append(p);
			qDebug() <<  p.name << p.time << p.common;
		}	
//		smaz(p);
		return true;
	}

AU:
//	smaz(p);
	return false;
}
