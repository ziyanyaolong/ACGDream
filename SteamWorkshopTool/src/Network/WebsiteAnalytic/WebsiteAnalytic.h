#pragma once

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QNetworkReply>
#include <QMetaType>

#include <ADDLib/JsonOperation.h>

#include "../../StaticData/SteamGet/SteamGet.h"
#include "../../ModDataTable/ModDataTable.h"

class WebsiteAnalytic : public QObject
{
	Q_OBJECT

public:

private:
	QString* webData;
	QVector<ModDataTable*> finishData;
	QString unicodeToUtf8(const QString& str);

public:
	explicit WebsiteAnalytic(QObject *parent = Q_NULLPTR);
	virtual ~WebsiteAnalytic();

	QVector<ModDataTable*>& getModDataTabelList() { return finishData; }

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