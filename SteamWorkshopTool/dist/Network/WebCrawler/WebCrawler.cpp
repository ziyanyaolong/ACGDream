#include "WebCrawler.h"

WebCrawler::WebCrawler(QObject *parent)
	: QObject(parent)
{
	web = new QNetworkAccessManager(this);
	webAddress = new QString("");
	webData = new QByteArray("");
	connect(web, &QNetworkAccessManager::finished, this, &WebCrawler::setWebsiteData, Qt::QueuedConnection);
	connect(this, &WebCrawler::websiteLink, this, &WebCrawler::startWebsiteLink, Qt::QueuedConnection);
}

WebCrawler::~WebCrawler()
{
	if (web)
		web->deleteLater();

	if (webAddress)
		delete webAddress;

	if (webData)
		delete webData;
}

void WebCrawler::startWebsiteLink(const QString& address)
{
	web->get(QNetworkRequest(QUrl(address)));
}

void WebCrawler::setWebsiteData(QNetworkReply* reply)
{
	*webData = reply->readAll();
	reply->deleteLater();
	emit this->finish(*webData);
}