#pragma once

#include <QWidget>
#include "ui_Settings.h"

class Settings : public QWidget
{
	Q_OBJECT

public:
	explicit Settings(QWidget *parent = Q_NULLPTR);
	virtual ~Settings();

private:
	Ui::Settings ui;
};
