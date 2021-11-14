#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	gui = new SWTGUI();
	webCrawler = new WebCrawler(this);
	websiteAnalytic = new WebsiteAnalytic(this);
	database = new DataBase(this);
	addGui(gui);

	connect(gui, &SWTGUI::haveWebAddress, webCrawler, &WebCrawler::websiteLink, Qt::QueuedConnection);

	void (WebsiteAnalytic:: * analyticWebsiteData)(const QByteArray & data) = &WebsiteAnalytic::analyticWebsiteData;

	connect(webCrawler, &WebCrawler::finish, websiteAnalytic, analyticWebsiteData);
	connect(websiteAnalytic, &WebsiteAnalytic::newModDataTable, this, [&](const ModDataTable& i) {
		database->addModDataTable(i, DataBase::Way::Overlay);
		});
	connect(websiteAnalytic, &WebsiteAnalytic::newModDataTable, gui, &SWTGUI::addMod, Qt::QueuedConnection);
}

SteamWorkshopTool::~SteamWorkshopTool()
{

	if (websiteAnalytic)
		websiteAnalytic->deleteLater();

	if (webCrawler)
		webCrawler->deleteLater();

	if (database)
		database->deleteLater();

	
}

void SteamWorkshopTool::pRun()
{
}