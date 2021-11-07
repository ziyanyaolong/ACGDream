#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	gui = new SWTGUI();
	webCrawler = new WebCrawler(this);
	websiteAnalytic = new WebsiteAnalytic(this);
	addGui(gui);

	connect(gui, &SWTGUI::haveWebAddress, webCrawler, &WebCrawler::websiteLink, Qt::QueuedConnection);

	void (WebsiteAnalytic:: * analyticWebsiteData)(const QString & data) = &WebsiteAnalytic::analyticWebsiteData;

	connect(webCrawler, &WebCrawler::finish, websiteAnalytic, analyticWebsiteData);
	connect(websiteAnalytic, &WebsiteAnalytic::finish, this, [&](auto data) {
		emit gui->printInfo(data[0]);
		});
}

SteamWorkshopTool::~SteamWorkshopTool()
{

	if (websiteAnalytic)
		websiteAnalytic->deleteLater();

	if (webCrawler)
		webCrawler->deleteLater();

	
}

void SteamWorkshopTool::pRun()
{
}