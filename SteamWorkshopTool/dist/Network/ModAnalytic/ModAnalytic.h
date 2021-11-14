#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include "../WebCrawler/WebCrawler.h"
#include "../WebsiteAnalytic/WebsiteAnalytic.h"

class ModAnalytic : public QObject
{
	Q_OBJECT

private:
	WebCrawler* webCrawler = nullptr;
	WebsiteAnalytic* websiteAnalytic = nullptr;
	QVector<ModDataTable*> modList;
	void (WebsiteAnalytic::* analyticWebsiteData)(const QByteArray& data) = &WebsiteAnalytic::analyticWebsiteData;

private:
	void closeWebModule();
	void startWebModule();

public:
	explicit ModAnalytic(QObject *parent = Q_NULLPTR);
	virtual ~ModAnalytic();
	const QVector<ModDataTable*>& readModList() { return modList; }

signals:
	void finish();
	void finished(const QVector<ModDataTable*>&);

public slots:
	void analyticMods(const QString& http);
	void clearModList();

private slots:
	void getMods(const QVector<ModDataTable*>& mods);

};
