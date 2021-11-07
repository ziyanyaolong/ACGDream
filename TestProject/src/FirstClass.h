#pragma once

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
//#include <QWebEngineView>

#include "ui_FirstClass.h"

#include "../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class mui : public QWidget
{
	Q_OBJECT

public:
	mui() : QWidget() { ui.setupUi(this); }
	Ui::FirstClass ui;
};

class _declspec(dllexport) FirstClass : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("FirstClass.json")

public:
	FirstClass(QObject *parent = Q_NULLPTR);
	~FirstClass();
	void createFile(QString filePath, QString fileName);
	virtual void pRun() override {  }

private:
	//QWebEngineView* view;
	mui* _mui_;
};


