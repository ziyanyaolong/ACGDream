#pragma once

#include "../GUI/SWTGUI.h"
#include "../Network/WebCrawler/WebCrawler.h"
#include "../Network/WebsiteAnalytic/WebsiteAnalytic.h"
#include "../../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"
#include "../DataBase/DataBase.h"

#include <QMetaType>

#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	WebCrawler* webCrawler = nullptr;
	WebsiteAnalytic* websiteAnalytic = nullptr;
	DataBase* database = nullptr;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;

signals:
	void haveNewWebDataTable(const QString&);

public slots:
};