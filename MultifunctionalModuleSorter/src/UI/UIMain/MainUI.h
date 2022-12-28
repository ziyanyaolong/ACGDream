#pragma once

#include <QWidget>
#include "ui_MainUI.h"

#include "../Components/DesignWidget/DesignWidget.h"

class MainUI : public QWidget
{
	Q_OBJECT

public:
	MainUI(QWidget *parent = nullptr);
	virtual ~MainUI();
	RectangleBlock b;

private:
	Ui::MainUI ui;
};
