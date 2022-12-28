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

#include "ui_SWTUI.h"
#include "ModSaveData/ModSaveData.h"
#include "../StaticData/StaticData.h"

class SWTUI : public QWidget
{
	Q_OBJECT

private:
	Ui::SteamWorkshopTool ui;
	QTimer* delayPushW = nullptr;
	QTimer* delayPushL = nullptr;

	void metaTypeRegister();

public:
	SWTUI(QWidget *parent = Q_NULLPTR);
	virtual ~SWTUI();
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
		QMessageBoxButtonData() {}
		QMessageBoxButtonData(const QString& _str, QMessageBox::ButtonRole _buttonRole, bool _isEmitSignal, const QObject* _receiver) : str(_str), buttonRole(_buttonRole), isEmitSignal(_isEmitSignal), receiver(_receiver) {}
		
		QString str = "";
		QMessageBox::ButtonRole buttonRole = QMessageBox::ButtonRole::InvalidRole;
		bool isEmitSignal = false;
		const QObject* receiver = nullptr;
	};

	void addEventFilterAllWidget();

protected:
	virtual bool eventFilter(QObject* target, QEvent* event) override;

signals:
	void loadList(const QStringList&, SWTUI::ListWay);
	void subscription(bool, const QString&, SWTUI::ListWay way);
	void importData(const QString&);
	void exportData(const QString&);
	void clearCache();
	void updateMod();
	void haveNewItemWidget();
	void addModReturn(ListWidgetItemWidget*, const ModDataTable&, SWTUI::ListWay);
	void clearLocalList();
	void clearWebsiteList();
	void messageBoxButtonConnect(const QObject* receiver, qint64 id, QPushButton* pushButton);
	void startSteamCMD();

public slots:
	void printInfo(const QString& data) { ui.textBrowser->insertPlainText(QString(data + "\n")); }
	void addMod(const ModDataTable& mod, SWTUI::ListWay into);
	void clearModList(SWTUI::ListWay way);
	void messageBox(const QString& title, const QString& text, QMessageBox::Icon icon, const QVector<SWTUI::QMessageBoxButtonData>& pushButtonList = QVector<SWTUI::QMessageBoxButtonData>());
	void refresh(SWTUI::ListWay way);
};
