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

	void (WebsiteAnalytic:: * analyticWebsiteData)(const QString & data) = &WebsiteAnalytic::analyticWebsiteData;

	connect(webCrawler, &WebCrawler::finish, websiteAnalytic, analyticWebsiteData);
	connect(websiteAnalytic, &WebsiteAnalytic::newModDataTable, this, [&](const ModDataTable& data) {
		/*if (data.size() > 0)
		{
			qDebug() << data[0].title;
			emit gui->printInfo(data[0].title);
		}*/
		emit database->addModDataTable(&data);
		emit this->haveNewWebDataTable(QString("name:" + data.title));
		});
	connect(this, &SteamWorkshopTool::haveNewWebDataTable, gui, &SWTGUI::printInfo, Qt::QueuedConnection);
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