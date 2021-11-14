#pragma once

#include <QtGui>
#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>
#include <QByteArray>
#include <QDebug>
#include <QVector>

class WebCrawler : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager* web = nullptr;
	QByteArray* webData = nullptr;
	QString* webAddress = nullptr;

public:
	WebCrawler(QObject *parent = Q_NULLPTR);
	~WebCrawler();
	QVector<void*> otherData;

	const QString& getWebData() { return *webData; }

signals:
	void finish(const QByteArray& data);
	void websiteLink(const QString& address);
	
private slots:
	void setWebsiteData(QNetworkReply* reply);
	void startWebsiteLink(const QString& address);

};
