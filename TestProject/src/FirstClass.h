#pragma once

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QDebug>

#include "ui_FirstClass.h"

class FirstClass : public QWidget
{
	Q_OBJECT

public:
	FirstClass(QWidget *parent = Q_NULLPTR);
	~FirstClass();
	void createFile(QString filePath, QString fileName);

private:
	Ui::FirstClass ui;
};
