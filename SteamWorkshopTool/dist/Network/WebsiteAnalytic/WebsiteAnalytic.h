#pragma once

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QNetworkReply>
#include <QMetaType>

#include "../../StaticData/SteamGet/SteamGet.h"
#include "../../ModDataTable/ModDataTable.h"
#include "../../JsonOperation/JsonOperation.h"

//"https://steamcommunity.com/app/294100/workshop/"
class WebsiteAnalytic : public QObject
{
	Q_OBJECT

public:

private:
	QString* webData;
	QVector<ModDataTable*> finishData;

public:
	WebsiteAnalytic(QObject *parent = Q_NULLPTR);
	~WebsiteAnalytic();

	QString unicodeToUtf8(const QString& str);

signals:
	void finish();
	void finished(const QVector<ModDataTable*>&);
	void newModDataTable(const ModDataTable&);

public slots:
	void analyticWebsiteData();
	void analyticWebsiteData(const QString& data);
	void analyticWebsiteData(const QByteArray& data) { analyticWebsiteData(QString(data)); }
	void clearModList()
	{
		qDeleteAll(finishData.begin(), finishData.end());
		finishData.clear();
	}

};