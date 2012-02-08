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
#include <QSlider>
#include <QGridLayout>
#include <vector>
#include "tab.h"
#include "timer.h"
#include "json.h"
#define MAX_TIME 999

Tab::Tab(QWidget *parent, QString name, QString path):QWidget(parent){
	
	setToolTip(name);

	if (parse(path)) {

		QVBoxLayout* vbox = new QVBoxLayout(this);
		vbox->setContentsMargins(30,30,30,30); 

		
		QHBoxLayout* hbox = new QHBoxLayout();
		hbox->setContentsMargins(0,0,0,0);

		QLabel* label = new QLabel("Celkovy cas:", this);
		hbox->addWidget(label);
		int sum = 0;
		for (int i = 0; i < parts.size(); i++)
			sum += parts[i]->origTime;

		totalTimeSpinBox = new QSpinBox(this);
		totalTimeSpinBox->setValue(sum);	
		totalTimeSpinBox->setSuffix(" min");	
		hbox->addWidget(totalTimeSpinBox);

		vbox->addLayout(hbox);

		QGridLayout *grid = new QGridLayout();
		
		for (int i = 0; i < parts.size(); i++){
			parts[i]->position = i; 
			parts[i]->parent = this; 

			parts[i]->checkBox = new QCheckBox (parts[i]->name,this);
			if ( parts[i]->chosen )
				parts[i]->checkBox->setCheckState(Qt::Checked);
			grid->addWidget(parts[i]->checkBox,i,0);
		
			parts[i]->slider = new QSlider(this);
			parts[i]->slider->setOrientation(Qt::Horizontal);
			grid->addWidget(parts[i]->slider,i,1);
			connect(parts[i]->slider, SIGNAL(valueChanged(int)), parts[i], SLOT(onChange(int)));

		
			parts[i]->label = new QLabel(QString::number(parts[i]->origTime)+" min", this);
			grid->addWidget(parts[i]->label,i,2);

		}

		vbox->addLayout(grid);

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

void PartMassage::onChange(int newValue){
	parent->changeTimeTable(position, newValue);	
}

void Tab::changeTimeTable(int part, int newValue){
	qDebug() << "pohnuto" << part << newValue;
}

void Tab::onOk(){
	QVector<pbInfo> v;
	for (int i = 0; i < parts.size(); i++){
		if (parts[i]->checkBox->checkState() == Qt::Checked){
			pbInfo pbi;
			pbi.name = parts[i]->name;
			pbi.time = parts[i]->time;
			v.append(pbi);

		} 

	}
//	Timer *timer = new Timer(v);
	new Timer(v);
}

int Tab::parse(QString path){

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QString in = QTextStream(&file).readAll();

	Json config(in);

	if (config.type != VECTOR)
		return false;
	for (int i = 0; i < (int) config.value.vector->size(); i++){

			Json v = (*config.value.vector)[i];
			if (v.type != MAP)
				return false;

			QMap<QString,Json> m = *v.value.map;
			PartMassage* p = new PartMassage();

			//name
			if ( m.find("name") == m.end())
				return false;
			if ( m["name"].type != STRING )
				return false;
			p->name = *m["name"].value.string;

			//time
			if ( m.find("time") == m.end())
				return false;
			if ( m["time"].type != INT )
				return false;
			int time = m["time"].value.number;
			if ( time <=0 || time >= MAX_TIME)
				return false;
			p->time = time;
			p->origTime = time;

			//chosen
			if ( m.find("default") == m.end())
				return false;
			switch ( m["default"].type ){
			case MY_TRUE:
				p->chosen = true;
				break;
			case MY_FALSE:
				p->chosen = false;
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
