#include "WebCrawler.h"

WebCrawler::WebCrawler(QObject *parent)
	: QObject(parent)
{
	web = new QNetworkAccessManager(this);
	webAddress = new QString("");
	webData = new QByteArray("");
	connect(web, &QNetworkAccessManager::finished, this, &WebCrawler::setWebsiteData, Qt::QueuedConnection);
	connect(this, &WebCrawler::websiteLink, this, &WebCrawler::startWebsiteLink);
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
	auto errorTemp = reply->error();
	if (errorTemp != QNetworkReply::NetworkError::NoError)
	{
		reply->deleteLater();
		emit this->error(errorTemp);
		return;
	}
	*webData = reply->readAll();
	reply->deleteLater();
	emit this->finish();
	emit this->finished(*webData);
}