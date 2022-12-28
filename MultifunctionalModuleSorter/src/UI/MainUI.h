#pragma once

#include <QWidget>
#include "ui_MainUI.h"

class MainUI : public QWidget
{
	Q_OBJECT

public:
	MainUI(QWidget *parent = nullptr);
	virtual ~MainUI();

private:
	Ui::MainUI ui;
};
