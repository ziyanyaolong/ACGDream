#pragma once

#include <QObject>
#include <QString>
#include <QRegExp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

//"https://steamcommunity.com/app/294100/workshop/"
class WebsiteAnalytic : public QObject
{
	Q_OBJECT

private:
	QString* webData;
	QVector<QString> finishData;

public:
	WebsiteAnalytic(QObject *parent = Q_NULLPTR);
	~WebsiteAnalytic();

signals:
	void finish(const QVector<QString>&);

public slots:
	void analyticWebsiteData();
	void analyticWebsiteData(const QString& data);

};
