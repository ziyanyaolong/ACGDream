#include "WebCrawler.h"

WebCrawler::WebCrawler(QObject *parent)
	: QObject(parent)
{
	codec = QTextCodec::codecForName("utf8");
	web = new QNetworkAccessManager(this);
	webData = new QString("");
	webAddress = new QString("");
	connect(web, &QNetworkAccessManager::finished, this, &WebCrawler::setWebsiteData);
	connect(this, &WebCrawler::websiteLink, this, &WebCrawler::startWebsiteLink);
}

WebCrawler::~WebCrawler()
{
	if (webAddress)
		delete webAddress;

	if (webData)
		delete webData;

	if (web)
		delete web;
}

void WebCrawler::startWebsiteLink(const QString& address)
{
	web->get(QNetworkRequest(QUrl(address)));
}

void WebCrawler::setWebsiteData(QNetworkReply* reply)
{
	*webData = codec->toUnicode(reply->readAll());
	reply->deleteLater();
	emit this->finish(*webData);
}