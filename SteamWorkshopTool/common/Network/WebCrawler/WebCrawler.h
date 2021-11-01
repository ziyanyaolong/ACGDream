#pragma once

#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QTextCodec>

class WebCrawler : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager* web;
	QString* webData;

public:
	WebCrawler(QObject *parent = Q_NULLPTR);
	~WebCrawler();

signals:
	void finish();

public slots:
	QString& getWebData() { return *webData; }

private slots:
	void setWebData(QNetworkReply* reply);

};
