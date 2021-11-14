#pragma once

#include "../GUI/SWTGUI.h"
#include "../Network/ModAnalytic/ModAnalytic.h"
#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#include "../DataBase/DataBase.h"
#include "../GUI/Components/ListWidgetItemWidget/ListWidgetItemWidget.h"

#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	ModAnalytic* modAnalytic = nullptr;
	DataBase* database = nullptr;
	QString steamMain = QString("https://steamcommunity.com/workshop/browse/?appid=");
	QString steamPage = QString("&actualsort=trend&p=");

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;

signals:
	void signalGui_addMod(const ModDataTable&);
	void signalDataBass_save(const ModDataTable&, DataBase::Way);

public slots:

};