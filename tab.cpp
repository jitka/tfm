#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
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
		sum = 0;
		for (int i = 0; i < parts.size(); i++){
			if (parts[i]->chosen)
				sum += parts[i]->origTime;
		}

		totalTimeSpinBox = new QSpinBox(this);
		totalTimeSpinBox->setValue(sum);	
		totalTimeSpinBox->setMaximum(MAX_TIME);	
		totalTimeSpinBox->setSuffix(" min");
		connect(totalTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSumChange(int)));
		hbox->addWidget(totalTimeSpinBox);

		vbox->addLayout(hbox);

		QGridLayout *grid = new QGridLayout();
		
		for (int i = 0; i < parts.size(); i++){
			parts[i]->position = i; 
			parts[i]->parent = this; 

			parts[i]->checkBox = new QCheckBox (parts[i]->name,this);
			if ( parts[i]->chosen )
				parts[i]->checkBox->setCheckState(Qt::Checked);
			connect(parts[i]->checkBox, SIGNAL(stateChanged(int)), parts[i], SLOT(onCheck(int)));
			grid->addWidget(parts[i]->checkBox,i,0);
		
			parts[i]->slider = new QSlider(this);
			parts[i]->slider->setOrientation(Qt::Horizontal);
			parts[i]->slider->setMinimum(0);
			parts[i]->slider->setSingleStep(1);
			parts[i]->slider->setPageStep(60);
			parts[i]->slider->setMaximum(sum*60);
			parts[i]->slider->setSliderPosition(parts[i]->time);
			connect(parts[i]->slider, SIGNAL(valueChanged(int)), parts[i], SLOT(onChange(int)));
			grid->addWidget(parts[i]->slider,i,1);

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
		QVBoxLayout* vbox = new QVBoxLayout(this);
		QLabel *label = new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		vbox->addWidget(label);
	}


}

void PartMassage::onChange(int newValue){
	parent->sliderChange(position, newValue);
}


void PartMassage::onCheck(int state){
	if (state == Qt::Checked)
		chosen = true;
	if (state == Qt::Unchecked)
		chosen = false;
	parent->sliderChange(position,time);
}

void Tab::sliderChange(int part, int newValue){
	if (parts[part]->time == newValue)
	 	return; //nic se nezmenilo
	if (!parts[part]->chosen)
	 	return; //nezajima me to
	int free = parts[part]->time - newValue;
	int used = 0;
	for (int i = 0; i < parts.size(); i++){
		if (i == part || !parts[i]->chosen)
			continue;
		used += free*parts[i]->time/(sum*60);
		parts[i]->time += free*parts[i]->time/(sum*60);
	}
	int rest = free-used;
	if (rest > 0)
		for (int i = 0; i < rest; i++){
			if (i == part || !parts[i % parts.size()]->chosen){
				rest++;
				continue;
			}
			parts[i % parts.size()]->time++;
		}
	if (rest < 0)
		for (int i = 0; i < -rest; i++){
			if (i == part || !parts[i % parts.size()]->chosen){
				rest--;
				continue;
			}
				continue;
			parts[i % parts.size()]->time--;
		}
	parts[part]->time = newValue;
	changeTimeTable();

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
	new Timer(v);
}

void Tab::onSumChange(int newValue){
	int free = (newValue - sum)*60;
	int used = 0;
	for (int i = 0; i < parts.size(); i++){
		if (!parts[i]->chosen)
			continue;
		used += free*parts[i]->time/(sum*60);
		parts[i]->time += free*parts[i]->time/(sum*60);
	}
	int rest = free-used;
	if (rest > 0)
		for (int i = 0; i < rest; i++){
			if (!parts[i % parts.size()]->chosen){
				rest++;
				continue;
			}
			parts[i % parts.size()]->time++;
		}
	if (rest < 0)
		for (int i = 0; i < -rest; i++){
			if (!parts[i % parts.size()]->chosen){
				rest--;
				continue;
			}
			parts[i % parts.size()]->time--;
		}
	sum = newValue;
	changeTimeTable();
}

void Tab::changeTimeTable(){
	for (int i = 0; i < parts.size(); i++){
		if (!parts[i]->chosen)
			continue;
		if (parts[i]->slider->maximum() < sum*60){
			parts[i]->slider->setMaximum(sum*60);
			parts[i]->slider->setSliderPosition(parts[i]->time);
		} else {
			parts[i]->slider->setSliderPosition(parts[i]->time);
			parts[i]->slider->setMaximum(sum*60);
		}
		parts[i]->label->setText(QString::number((parts[i]->time+30)/60)+" min");
	}

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
			p->time = time*60;
			p->origTime = time;

			//chosen
			if ( m.find("chosen") == m.end())
				return false;
			switch ( m["chosen"].type ){
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
