#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#elif _MSC_VER >= 1600
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
#include <QPainter>
#include <QRect>

#include "ui_SWTGUI.h"
#include "ModSaveData/ModSaveData.h"
#include "../StaticData/StaticData.h"

class SWTGUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SteamWorkshopTool ui;
	QTimer* delayPushW = nullptr;
	QTimer* delayPushL = nullptr;

	void metaTypeRegister();

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

	struct QMessageBoxButtonData
	{
		QString str = "";
		QMessageBox::ButtonRole buttonRole = QMessageBox::ButtonRole::InvalidRole;
		bool isEmitSignal = false;
		const QObject* receiver = nullptr;
	};

	void addEventFilterAllWidget();

protected:
	virtual bool eventFilter(QObject* target, QEvent* event) override;

signals:
	void loadList(const QStringList&, SWTGUI::ListWay);
	void subscription(bool, const QString&, SWTGUI::ListWay way);
	void importData(const QString&);
	void exportData(const QString&);
	void clearCache();
	void updateMod();
	void haveNewItemWidget();
	void addModReturn(ListWidgetItemWidget*, const ModDataTable&, SWTGUI::ListWay);
	void clearLocalList();
	void clearWebsiteList();
	void messageBoxButtonConnect(const QObject* receiver, qint64 id, QPushButton* pushButton);
	void startSteamCMD();

public slots:
	void printInfo(const QString& data) { ui.textBrowser->insertPlainText(QString(data + "\n")); }
	void addMod(const ModDataTable& mod, SWTGUI::ListWay into);
	void clearModList(SWTGUI::ListWay way);
	void messageBox(const QString& title, const QString& text, QMessageBox::Icon icon, const QVector<SWTGUI::QMessageBoxButtonData>& pushButtonList = QVector<SWTGUI::QMessageBoxButtonData>());
	void refresh(SWTGUI::ListWay way);
};
