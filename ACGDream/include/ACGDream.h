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
#include <QPluginLoader>
#include <QMessageBox>
#include <QEventLoop>
#include <QTextCodec>

#include "../ACGDreamFrame/include/ACGDreamFrameH.h"
#pragma comment (lib,"../.build/vs/ACGDreamFrame/lib/ACGDreamFrame.lib")

#include "../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
//#pragma comment (lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class ACGDream : public ACGDreamFrame
{
	Q_OBJECT

private:
	PluginCalInterface* m_pInterface = nullptr;

protected:
	virtual void closeEvent(QCloseEvent* e);

public:
	ACGDream(QMainWindow*parent = Q_NULLPTR);
	~ACGDream();
	bool loadPluginAll();

};