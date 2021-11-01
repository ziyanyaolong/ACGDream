#pragma once

#include <QObject>
#include <QWidget>
#include <Qdebug>

#include "ui_SteamWorkshopTool.h"

#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"

//#pragma comment(lib,"../../../.build/vs/ACGDreamLoadPlugs/libACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		Q_INTERFACES(PluginCalInterface)
		Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE "SteamWorkshopTool.json")
public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void run() override;

private:
	Ui::SteamWorkshopTool ui;
	
};