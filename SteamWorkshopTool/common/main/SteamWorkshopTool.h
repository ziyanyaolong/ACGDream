#pragma once

#include <QObject>
#include <Qdebug>

#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
//#pragma comment(lib,"../../../.build/vs/ACGDreamLoadPlugs/libACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public QObject, public PluginCalInterface
{
	Q_OBJECT
		Q_INTERFACES(PluginCalInterface)
		Q_PLUGIN_METADATA(IID PluginCalInterface_iid FILE "plugCalplugin.json")
public:
	SteamWorkshopTool();
	virtual int run(int, char* [], ACGDream const*) override;

private:
};