#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	gui = new SWTGUI();
	webCrawler = new WebCrawler(this);
	websiteAnalytic = new WebsiteAnalytic(this);
	addGui(gui);

	connect(gui, &SWTGUI::haveWebAddress, webCrawler, &WebCrawler::websiteLink);

	void (WebsiteAnalytic:: * analyticWebsiteData)(const QString & data) = &WebsiteAnalytic::analyticWebsiteData;

	connect(webCrawler, &WebCrawler::finish, websiteAnalytic, analyticWebsiteData);
	connect(websiteAnalytic, &WebsiteAnalytic::finish, this, [&](auto data) {
		emit gui->printInfo(data[0]);
		});
}

SteamWorkshopTool::~SteamWorkshopTool()
{
	if (gui)
	{
		gui->close();
		delete gui;
	}

	if (webCrawler)
		delete webCrawler;

	if (websiteAnalytic)
		delete websiteAnalytic;
}

void SteamWorkshopTool::run()
{
	setOpenEventLoop(true);
}