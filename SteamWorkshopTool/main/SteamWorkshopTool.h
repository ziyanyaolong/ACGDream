#pragma once

#include "../common/GUI/SWTGUI.h"
#include "../common/Network/WebCrawler/WebCrawler.h"
#include "../common/Network/WebsiteAnalytic/WebsiteAnalytic.h"
#include "../../ACGDreamLoadPlugs/include/ACGDreamLoadPlugs.h"

#pragma comment(lib,"../.build/vs/ACGDreamLoadPlugs/lib/ACGDreamLoadPlugs.lib")

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	WebCrawler* webCrawler = nullptr;
	WebsiteAnalytic* websiteAnalytic = nullptr;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;

signals:


	
};