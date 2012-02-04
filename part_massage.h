#ifndef PART_MASSAGE_H
#define PART_MASSAGE_H
#include <QCheckBox>
#include <QSpinBox>
#include <QProgressBar>

struct part_massage{
	QString name;
	int time;
	int final_time;
	bool chosen;
	bool final_chosen;

	QCheckBox* checkBox;
	QSpinBox* spinBox;
	QProgressBar* progresBar;
};

#endif
