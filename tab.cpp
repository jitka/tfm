#include <QFile>
#include <QTextStream>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDebug>
#include "tab.h"
#include "timer.h"
#include "json.h"
#define MAX_TIME 999

Tab::Tab(QWidget *parent, QString name, QString path):QWidget(parent){
	
	setToolTip(name);

	if (parse(path)) {

		QVBoxLayout* vbox = new QVBoxLayout(this);
		vbox->setContentsMargins(30,30,30,30); 

		{		
			QHBoxLayout* hbox = new QHBoxLayout();
			hbox->setContentsMargins(0,0,0,0);

			QLabel* label = new QLabel("Celkovy cas:", this);
			hbox->addWidget(label);
			origSum = 0;
			for (int i = 0; i < parts.size(); i++){
				if (parts[i]->origChosen)
					origSum += parts[i]->origTime;
			}
			sum = origSum;

			totalTimeSpinBox = new QSpinBox(this);
			totalTimeSpinBox->setMinimum(1);	
			totalTimeSpinBox->setMaximum(MAX_TIME);	
			totalTimeSpinBox->setValue(sum);	
			totalTimeSpinBox->setSuffix(" min");
			connect(totalTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSumChange(int)));
			hbox->addWidget(totalTimeSpinBox);

			vbox->addLayout(hbox);
		}
		
		QGridLayout *grid = new QGridLayout();
		for (int i = 0; i < parts.size(); i++){
			parts[i]->position = i; 
			parts[i]->parent = this; 

			parts[i]->checkBox = new QCheckBox (parts[i]->name,this);
			parts[i]->chosen = parts[i]->origChosen;
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

			parts[i]->min = new QLabel(QString::number(parts[i]->origTime), this);
			grid->addWidget(parts[i]->min,i,2);
			
			QLabel* min = new QLabel("min", this);
			grid->addWidget(min,i,3);
			
			parts[i]->sec = new QLabel("0", this);
			grid->addWidget(parts[i]->sec,i,4);
			
			QLabel* sec = new QLabel("sek", this);
			grid->addWidget(sec,i,5);
		}

		vbox->addLayout(grid);

		{
			QHBoxLayout* hbox = new QHBoxLayout();
			hbox->setContentsMargins(0,0,0,0);

			QPushButton* ok = new QPushButton("zacit masirovat", this);
			connect(ok, SIGNAL(clicked()), this, SLOT(onOk()));
			hbox->addWidget(ok);

			QPushButton* reset = new QPushButton("reset", this);
			connect(reset, SIGNAL(clicked()), this, SLOT(onReset()));
			hbox->addWidget(reset);

			vbox->addLayout(hbox);
		}

	} else {
		//nepovedlo se nacit konfigurak
		setToolTip(name);
		QVBoxLayout* vbox = new QVBoxLayout(this);
		QLabel* label = new QLabel("Nepodarilo se naparsovat konfigurak.", this);
		vbox->addWidget(label);
	}


}

void PartMassage::onChange(int newValue){
	//slouzi pouze k propakovani signalu dal
	parent->sliderChange(position, newValue);
}


void PartMassage::onCheck(int state){
	//slouzi pouze k propakovani signalu dal
	if (state == Qt::Checked)
		chosen = true;
	if (state == Qt::Unchecked)
		chosen = false;
	parent->sliderChange(position,time);
}

void Tab::sliderChange(int part, int newValue){
	//Zmenila se priorita nejake casti mazase. Cas ktery se na ni ztrati
	//nebo ziska se rozdeli mezi ostatni vybrane casti tak aby vetsi casti 
	//ziskaly/ztratily vic.
	//Celkova doba masaze zustava na sekundy presna.
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
	int i = 0;
	while (rest > 0){
		i = (i + 1) % parts.size();
		if (i == part || !parts[i]->chosen){
			continue;
		}
		parts[i]->time++;
		rest--;
	}
	while (rest < 0){
		i = (i + 1) % parts.size();
		if (i == part || !parts[i]->chosen || parts[i]->time == 0){
			continue;
		}
		parts[i]->time--;
		rest++;
	}
	parts[part]->time = newValue;
/*
	int sum2 = 0;
	for (int i = 0; i < parts.size(); i++){
		sum2 += parts[i]->time;
	}
	qDebug() << sum*60 << sum2;
*/		

	changeTimeTable();

}

void Tab::onOk(){
	//Zacina se masirovat, vytvorisi se samostatne okno s hodinami.
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
	//Zvetsila/zmensila se celkova doba masaze cas se pomerove rozdeli
	//mezi vsechny ostani vybrane casi.
	int free = (newValue - sum)*60;
	int used = 0;
	for (int i = 0; i < parts.size(); i++){
		if (!parts[i]->chosen)
			continue;
		used += free*parts[i]->time/(sum*60);
		parts[i]->time += free*parts[i]->time/(sum*60);
	}
	int rest = free-used;

	int i = 0;
	while (rest > 0){
		i = (i + 1) % parts.size();
		if (!parts[i]->chosen){
			continue;
		}
		parts[i]->time++;
		rest--;
	}
	while (rest < 0){
		i = (i + 1) % parts.size();
		if (!parts[i]->chosen || parts[i]->time == 0){
			continue;
		}
		parts[i]->time--;
		rest++;
	}

	sum = newValue;
	changeTimeTable();
}

void Tab::onReset(){
	sum = origSum;
	totalTimeSpinBox->setValue(sum);	
	for (int i = 0; i < parts.size(); i++){
		parts[i]->chosen = parts[i]->origChosen;
		if ( parts[i]->chosen )
			parts[i]->checkBox->setCheckState(Qt::Checked);

		parts[i]->slider->setMaximum(sum*60);
		parts[i]->time = parts[i]->origTime*60;
		parts[i]->slider->setSliderPosition(parts[i]->time);

		parts[i]->min->setText(QString::number(parts[i]->origTime));
		parts[i]->sec->setText("0");
	}
}


void Tab::changeTimeTable(){
	//Nove vypocitane casy se vykresli.
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
		parts[i]->min->setText(QString::number(parts[i]->time/60));
		parts[i]->sec->setText(QString::number(parts[i]->time%60));
	}

}

int Tab::parse(QString path){
	//Parsuje se konfigurat. Vrati se jestli se povedlo.

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
				p->origChosen = true;
				break;
			case MY_FALSE:
				p->origChosen = false;
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
