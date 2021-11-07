#include "WebCrawler.h"

WebCrawler::WebCrawler(QObject *parent)
	: QObject(parent)
{
	codec = QTextCodec::codecForName("utf8");
	web = new QNetworkAccessManager(this);
	webAddress = new QString("");
	webData = new QString("");
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
	*webData = codec->toUnicode(reply->readAll());
	reply->deleteLater();
	emit this->finish(*webData);
}