#include "WebsiteAnalytic.h"

WebsiteAnalytic::WebsiteAnalytic(QObject *parent)
	: QObject(parent)
{
	finishData.push_back("");
	webData = new QString("");
}

WebsiteAnalytic::~WebsiteAnalytic()
{
	if (webData)
		delete webData;
}

void WebsiteAnalytic::analyticWebsiteData(const QString& data)
{
	*webData = data;
	analyticWebsiteData();
}

void WebsiteAnalytic::analyticWebsiteData()
{
	finishData[0] = webData->mid(webData->indexOf("<div class=\"workshopBrowseItems\">"), webData->indexOf("<div class=\"workshopBrowsePaging\">"));
	emit this->finish(finishData);
}