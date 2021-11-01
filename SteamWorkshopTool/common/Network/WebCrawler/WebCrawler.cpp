#include "WebCrawler.h"

WebCrawler::WebCrawler(QObject *parent)
	: QObject(parent)
{
	web = new QNetworkAccessManager(this);
	webData = new QString("");
	connect(web, &QNetworkAccessManager::finished, this, &WebCrawler::setWebData);
}

WebCrawler::~WebCrawler()
{
	if (webData)
		delete webData;

	if (web)
		delete web;
}

void WebCrawler::setWebData(QNetworkReply* reply)
{
	QTextCodec* codec = QTextCodec::codecForName("utf8");
	*webData = codec->toUnicode(reply->readAll());
	reply->deleteLater();
	emit this->finish();
}