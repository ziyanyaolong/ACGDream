#pragma once

#include <QtGui>
#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class WebCrawler : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager* web = nullptr;
	QString* webData = nullptr;
	QString* webAddress = nullptr;
	QTextCodec* codec = nullptr;

public:
	WebCrawler(QObject *parent = Q_NULLPTR);
	~WebCrawler();

	const QString& getWebData() { return *webData; }

signals:
	void finish(const QString& data);
	void websiteLink(const QString& address);
	
private slots:
	void setWebsiteData(QNetworkReply* reply);
	void startWebsiteLink(const QString& address);

};
