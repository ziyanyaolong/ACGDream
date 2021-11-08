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

public:
	struct WebDataTable
	{
		QString image;
		uint64_t id;
		uint64_t appid;
		QString title;
	};

private:
	QString* webData;
	QVector<WebDataTable> finishData;
	QString head = QString("<div class=\"workshopItem\">");
	QString tail = QString("</script>");
	QString jsHead = QString("SharedFileBindMouseHover(");
	QString jsTail = QString(");");
	QStringList matchList;

public:
	WebsiteAnalytic(QObject *parent = Q_NULLPTR);
	~WebsiteAnalytic();

signals:
	void finish(const QVector<WebDataTable>&);

public slots:
	void analyticWebsiteData();
	void analyticWebsiteData(const QString& data);

};
