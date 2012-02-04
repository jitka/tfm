#include <QPushButton>
#include <fstream>
#include <iostream>
#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <QTextStream>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QButtonGroup>
#include <vector>
#include "tab.h"
#include "timer.h"
#include "json.h"
#define MAX_TIME 999

Tab::Tab(QWidget *parent, QString name):QWidget(parent){
	
	setToolTip(name);

	if (parse(name)) {

		QVBoxLayout* vbox = new QVBoxLayout(this);
		vbox->setContentsMargins(30,30,30,30); 

		
		QHBoxLayout* hbox = new QHBoxLayout();
		hbox->setContentsMargins(0,0,0,0);

		QLabel* label = new QLabel("Celkovy cas:", this);
		hbox->addWidget(label);
		int sum = 0;
		for (int i = 0; i < parts.size(); i++)
			sum += parts[i].time;

		QSpinBox* totalTimeSpinBox = new QSpinBox(this);
		totalTimeSpinBox->setValue(sum);	
		totalTimeSpinBox->setSuffix(" min");	
		hbox->addWidget(totalTimeSpinBox);

		vbox->addLayout(hbox);

		for (int i = 0; i < parts.size(); i++){
			QHBoxLayout *hbox = new QHBoxLayout();
			hbox->setContentsMargins(0,0,0,0);
			
			parts[i].checkBox = new QCheckBox (parts[i].name,this);
			if ( parts[i].chosen )
				parts[i].checkBox->setCheckState(Qt::Checked);
			hbox->addWidget(parts[i].checkBox);
		
			parts[i].spinBox = new QSpinBox(this);
		        parts[i].spinBox->setMaximum(MAX_TIME);	
		        parts[i].spinBox->setValue(parts[i].time);	
			hbox->addWidget(parts[i].spinBox);

			vbox->addLayout(hbox);

		}

		QPushButton *ok = new QPushButton("zacit masirovat", this);
		connect(ok, SIGNAL(clicked()), this, SLOT(onOk()));
		vbox->addWidget(ok);

	} else {
		//nepovedlo se nacit konfigurak
		setToolTip(name);
		//QLabel *label = new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		QPushButton *quit = new QPushButton("Quit", this);
		quit->setGeometry(50, 40, 75, 30);
	}


}

void Tab::onOk(){
	for (int i = 0; i < parts.size(); i++){
		if (parts[i].checkBox->checkState() == Qt::Checked)
			parts[i].final_chosen = true;
		else
			parts[i].final_chosen = false;
		parts[i].final_time = parts[i].spinBox->value();

	}
	Timer *timer = new Timer(parts);
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
			int time = (*m)["time"].hodnota.Int;
			if ( time <=0 || time >= MAX_TIME)
				goto AU;
			p.time = (*m)["time"].hodnota.Int;

			if ( (*m).find("default") == (*m).end())
				goto AU;
			switch ( (*m)["default"].typ ){
			case MY_TRUE:
				p.chosen = true;
				break;
			case MY_FALSE:
				p.chosen = false;
				break;
			default:
				goto AU;
				break;
			}

			parts.append(p);
//			qDebug() <<  p.name << p.time << p.chosen;
		}	
//		smaz(p);
		return true;
	}

AU:
//	smaz(p);
	return false;
}
