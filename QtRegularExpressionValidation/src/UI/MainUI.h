#pragma once

#include <QWidget>
#include "ui_MainUI.h"

class MainUI : public QWidget
{
	Q_OBJECT

public:
	MainUI(QWidget *parent = Q_NULLPTR);
	~MainUI();

private:
	friend class QtRegularExpressionValidation;
	Ui::MainUI ui;

public slots:
	void printText(const QString& str) { ui.textBrowser->setText(str); }
};
