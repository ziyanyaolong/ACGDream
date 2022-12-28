#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QMessageBox>

#include "../WebCrawler/WebCrawler.h"
#include "../WebsiteAnalytic/WebsiteAnalytic.h"

class ModAnalytic : public QObject
{
	Q_OBJECT

private:
	WebCrawler* webCrawler = nullptr;
	WebsiteAnalytic* websiteAnalytic = nullptr;
	QVector<ModDataTable*> modList;
	static void (WebsiteAnalytic::* analyticWebsiteData)(const QByteArray& data);

private:
	void closeWebModule();
	void startWebModule();

public:
	explicit ModAnalytic(QObject *parent = Q_NULLPTR);
	virtual ~ModAnalytic();
	const QVector<ModDataTable*>& readModList() { return modList; }
	ModDataTable* findMod(const QString& id);

signals:
	void finish();
	void finished(const QVector<ModDataTable*>&);
	void error(QNetworkReply::NetworkError);

public slots:
	void analyticMods(const QString& http);
	void clearModList();

private slots:
	void getMods(const QVector<ModDataTable*>& mods);

};
