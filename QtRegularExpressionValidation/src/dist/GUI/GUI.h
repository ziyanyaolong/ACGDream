#pragma once

#include <QWidget>
#include "ui_GUI.h"

class GUI : public QWidget
{
	Q_OBJECT

public:
	GUI(QWidget *parent = Q_NULLPTR);
	~GUI();

private:
	friend class QtRegularExpressionValidation;
	Ui::GUI ui;

public slots:
	void printText(const QString& str) { ui.textBrowser->setText(str); }
};
