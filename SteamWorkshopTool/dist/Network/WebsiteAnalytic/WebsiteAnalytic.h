#pragma once

#include <QObject>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QNetworkReply>
#include <QMetaType>

#include "../../ModDataTable/ModDataTable.h"

//"https://steamcommunity.com/app/294100/workshop/"
class WebsiteAnalytic : public QObject
{
	Q_OBJECT

public:

private:
	QString* webData;
	QVector<ModDataTable*> finishData;
	QString head = QString("<div class=\"workshopItem\">");
	QString tail = QString("</script>");
	QString jsHead = QString("SharedFileBindMouseHover(");
	QString jsTail = QString(");");
	QString imageHead = QString("<img class=\"workshopItemPreviewImage  aspectratio_16x9\" src=\"");
	QString imageTail = QString("\">");
	QStringList matchList;

public:
	WebsiteAnalytic(QObject *parent = Q_NULLPTR);
	~WebsiteAnalytic();

	QString unicodeToUtf8(const QString& str);

signals:
	void finish(const QVector<ModDataTable*>&);
	void newModDataTable(const ModDataTable&);

public slots:
	void analyticWebsiteData();
	void analyticWebsiteData(const QString& data);
	void analyticWebsiteData(const QByteArray& data) { analyticWebsiteData(QString(data)); }

};

//Q_DECLARE_METATYPE(WebsiteAnalytic::ModDataTable)