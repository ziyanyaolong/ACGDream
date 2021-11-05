#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QObject>
#include <QWidget>
#include <QString>
#include <Qdebug>
#include <QTextCodec>
#include <QPushButton>

#include "ui_SWTGUI.h"

class SWTGUI : public QWidget
{
	Q_OBJECT

public:
	SWTGUI(QWidget *parent = Q_NULLPTR);
	~SWTGUI();

signals:
	void haveWebAddress(const QString&);

private:
	Ui::SteamWorkshopTool ui;

private slots:
	void pushButtonResponse_WebAddress();

public slots:
	void printInfo(const QString& data) { ui.textBrowser->insertPlainText(data + "\n"); }

};
