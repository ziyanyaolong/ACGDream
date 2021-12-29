#pragma once

#include "QDebug"

#include "../StaticData/StaticData.h"
#include "../GUI/SWTGUI.h"
#include "../Network/ModAnalytic/ModAnalytic.h"
#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#include "../DataBase/DataBase.h"
#include "../GUI/Components/ListWidgetItemWidget/ListWidgetItemWidget.h"
#include "../LoadRes/LoadRes.h"

#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	ModAnalytic* modAnalytic = nullptr;
	DataBase* database = nullptr;
	LoadRes loadRes;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;
	void initPicture();

signals:

public slots:
	
};