#include <QPushButton>
#include <QFile>
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
#include "json2.h"
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

	QString n = "/home/jitka/.tfm/"+name;
	QFile file(n);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	QTextStream in(&file);
	
	QString in2 = in.readAll();

	Json config(in2);

	if (config.type != VECTOR)
		return false;
	for (int i = 0; i < (int) config.value.vector->size(); i++){

			Json v = (*config.value.vector)[i];
			if (v.type != MAP)
				return false;

			QMap<QString,Json> m = *v.value.map;
			part_massage p;

			//name
			if ( m.find("name") == m.end())
				return false;
			if ( m["name"].type != STRING )
				return false;
			p.name = *m["name"].value.string;

			//time
			if ( m.find("time") == m.end())
				return false;
			if ( m["time"].type != INT )
				return false;
			int time = m["time"].value.number;
			if ( time <=0 || time >= MAX_TIME)
				return false;
			p.time = time;

			//chosen
			if ( m.find("default") == m.end())
				return false;
			switch ( m["default"].type ){
			case MY_TRUE:
				p.chosen = true;
				break;
			case MY_FALSE:
				p.chosen = false;
				break;
			default:
				return false;
				break;
			}

			parts.append(p);
			//qDebug() <<  p.name << p.time << p.chosen;

	
	}

	return true;
}
