#pragma once

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QWebEngineView>

#include "ui_FirstClass.h"

#include "../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"

class _declspec(dllexport) FirstClass : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("FirstClass.json")

public:
	FirstClass(QWidget *parent = Q_NULLPTR);
	~FirstClass();
	void createFile(QString filePath, QString fileName);
	virtual void run() override { setOpenEventLoop(true); }

private:
	Ui::FirstClass ui;
	QWebEngineView view;
};
