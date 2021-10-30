#pragma once

#include <QWidget>
#include <QDebug>
#include <QMap>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>

#include "../ACGDreamFrame/include/ACGDreamFrameH.h"
#pragma comment (lib,"../.build/vs/ACGDreamFrame/lib/ACGDreamFrame.lib")

#include "../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
//#pragma comment (lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class ACGDream : public ACGDreamFrame
{
	Q_OBJECT

private:
	PluginCalInterface* m_pInterface = nullptr;

public:
	ACGDream(QWidget *parent = Q_NULLPTR);
	~ACGDream();
	bool loadPlugin();
	
};