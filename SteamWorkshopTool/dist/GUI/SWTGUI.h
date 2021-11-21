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
#include <QListWidgetItem>
#include <QCoreApplication>
#include <QDir>
#include <QVector>
#include <List>
#include <QStringList>
#include <QFileDialog>

#include "ui_SWTGUI.h"
#include "Components/ListWidgetItemWidget/ListWidgetItemWidget.h"
#include "../ModDataTable/ModDataTable.h"
#include "../Network/WebCrawler/WebCrawler.h"

class SWTGUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SteamWorkshopTool ui;
	QVector<QWidget*> list;
	QList<WebCrawler*> webList;
	QStringList datalist;

public:
	SWTGUI(QWidget *parent = Q_NULLPTR);
	~SWTGUI();

signals:
	void webAddress(const QStringList& list);
	void subscription(bool isSubscription, const QString& id);
	void importData(const QString& address);
	void exportData(const QString& address);
	void clearCache();
	void updateMod();

public slots:
	void printInfo(const QString& data) { ui.textBrowser->insertPlainText(QString(data + "\n")); }
	void addMod(const ModDataTable& mod);
	void clearModList();
};
