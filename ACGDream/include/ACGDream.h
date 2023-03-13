#pragma once

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QDebug>
#include <QMap>
#include <QDir>
#include <QMessageBox>
#include <QEventLoop>
#include <QTextCodec>
#include <QDir>
#include <QThread>

#include <ACGDreamFrameH.h>
#include <ACGDreamLoadPlugs.h>

class ACGDream : public ACGDreamFrame
{
	Q_OBJECT

private:
	PluginCore* pluginCore;
	QThread* pluginThread;

protected:
	virtual void closeEvent(QCloseEvent* e);

public:
	explicit ACGDream(QMainWindow* parent = Q_NULLPTR);
	virtual ~ACGDream();

public slots:
	void initDirs(QString name = "");

};