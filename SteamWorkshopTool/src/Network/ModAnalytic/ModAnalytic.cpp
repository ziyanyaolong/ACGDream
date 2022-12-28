#include "ModAnalytic.h"

void (WebsiteAnalytic::* ModAnalytic::analyticWebsiteData)(const QByteArray& data) = &WebsiteAnalytic::analyticWebsiteData;

ModAnalytic::ModAnalytic(QObject *parent)
	: QObject(parent)
{
}

ModAnalytic::~ModAnalytic()
{
	clearModList();

	if (webCrawler)
		webCrawler->deleteLater();

	webCrawler = nullptr;

	if (websiteAnalytic)
		websiteAnalytic->deleteLater();

	websiteAnalytic = nullptr;
}

inline void ModAnalytic::clearModList()
{
	qDeleteAll(modList.begin(), modList.end());
	modList.clear();
}

ModDataTable* ModAnalytic::findMod(const QString& id)
{
	foreach (auto i, modList)
	{
		if ((i->appid + i->id) == id)
			return i;
	}
	return nullptr;
}

void ModAnalytic::closeWebModule()
{
	if (webCrawler)
		webCrawler->deleteLater();

	if (websiteAnalytic)
		websiteAnalytic->deleteLater();

	webCrawler = nullptr;
	websiteAnalytic = nullptr;
}

inline void ModAnalytic::startWebModule()
{
	closeWebModule();
	webCrawler = new WebCrawler(this);
	websiteAnalytic = new WebsiteAnalytic(this);
}

void ModAnalytic::analyticMods(const QString& http)
{
	startWebModule();
	connect(webCrawler, &WebCrawler::error, this, [&](QNetworkReply::NetworkError errorData){
		closeWebModule();
		this->clearModList();
		emit this->error(errorData);
		});
	connect(webCrawler, &WebCrawler::finished, websiteAnalytic, analyticWebsiteData);
	connect(websiteAnalytic, &WebsiteAnalytic::finished, this, &ModAnalytic::getMods);

	emit webCrawler->websiteLink(http);
}

void ModAnalytic::getMods(const QVector<ModDataTable*>& mods)
{
	clearModList();
	foreach (ModDataTable * i, mods)
	{
		modList.push_back(new ModDataTable(*i));
	}
	closeWebModule();

	emit this->finish();
	emit this->finished(modList);
}