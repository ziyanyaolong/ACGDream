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
#include <QList>
#include <QTimer>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>

#include "ui_SWTGUI.h"
#include "ModSaveData/ModSaveData.h"

class SWTGUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SteamWorkshopTool ui;
	ModSaveData localModSaveData;
	ModSaveData webModSaveData;
	QTimer* delayPushW = nullptr;
	QTimer* delayPushL = nullptr;

public:
	SWTGUI(QWidget *parent = Q_NULLPTR);
	~SWTGUI();
	enum class Form {
		Information,
		Warning,
		Critical,
		Question,
		About
	};

	enum class ListWay
	{
		Website,
		Local,
		All
	};

signals:
	void loadList(const QStringList&, SWTGUI::ListWay);
	void subscription(bool, const QString&);
	void importData(const QString&);
	void exportData(const QString&);
	void clearCache();
	void updateMod();
	void haveNewItemWidget();

public slots:
	void printInfo(const QString& data) { ui.textBrowser->insertPlainText(QString(data + "\n")); }
	void addMod(const ModDataTable& mod, SWTGUI::ListWay into);
	void newItemWidget(const ModDataTable& mod);
	void clearModList(SWTGUI::ListWay way);
	void messageBox(SWTGUI::Form form, const QString& title, const QString& text, QMessageBox::StandardButton button0 = QMessageBox::StandardButton::Ok, QMessageBox::StandardButton button1 = QMessageBox::StandardButton::NoButton);

};
