#pragma once

#include <ACGDreamLoadPlugs.h>
#include <QDebug>

#include "../StaticData/StaticData.h"
#include "../GUI/SWTGUI.h"
#include "../Network/ModAnalytic/ModAnalytic.h"
#include "../DataBase/DataBase.h"
#include "../GUI/Components/ListWidgetItemWidget/ListWidgetItemWidget.h"
#include "../LoadRes/LoadRes.h"
#include "../Downloader/Downloader.h"


class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTGUI* gui = nullptr;
	ModAnalytic* modAnalytic = nullptr;
	DataBase* database = nullptr;
	Downloader* downloader = nullptr;
	LoadRes loadRes;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;
	void initPicture();

signals:

public slots:
	void clearCacheProcess();
	void clearCacheChoice();

	void updateModProcess();
	void addModProcess(ListWidgetItemWidget* item, const ModDataTable& mod, SWTGUI::ListWay way);

	void loadListProcess(const QStringList& list, SWTGUI::ListWay way);

	void subscriptionProcess(bool isSubscription, const QString& id, SWTGUI::ListWay way);

	void modAnalyticFinishedProcess(const QVector<ModDataTable*>& mods);
	void modAnalyticErrorProcess(QNetworkReply::NetworkError errorData);


	void messageBoxButtonProcess(const QObject* receiver, qint64 id, QPushButton* pushButton);
};