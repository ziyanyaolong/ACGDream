#pragma once

#include "../StaticData/SteamGet/SteamGet.h"
#include "../GUI/SWTGUI.h"
#include "../Network/ModAnalytic/ModAnalytic.h"
#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#include "../DataBase/DataBase.h"
#include "../GUI/Components/ListWidgetItemWidget/ListWidgetItemWidget.h"
#include "../JsonOperation/JsonOperation.h"

#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	ModAnalytic* modAnalytic = nullptr;
	DataBase* database = nullptr;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;

signals:
	void signalGui_addMod(const ModDataTable&);
	void signalDataBass_save(const ModDataTable*, DataBase::Way);
	void signalDataBass_delete(const QString& key);

public slots:

};