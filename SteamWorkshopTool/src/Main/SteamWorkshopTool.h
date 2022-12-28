#pragma once

#include <QDebug>

#include "../StaticData/StaticData.h"
#include "../UI/SWTUI.h"
#include "../Network/ModAnalytic/ModAnalytic.h"
#include "../DataBase/DataBase.h"
#include "../UI/Components/ListWidgetItemWidget/ListWidgetItemWidget.h"
#include "../LoadRes/LoadRes.h"
#include "../Downloader/Downloader.h"

#include <ACGDreamLoadPlugs.h>

class _declspec(dllexport) SteamWorkshopTool : public PluginCalInterface
{
	Q_OBJECT
		RegPlugin("SteamWorkshopTool.json")

private:
	SWTUI* ui = nullptr;
	ModAnalytic* modAnalytic = nullptr;
	DataBase* database = nullptr;
	Downloader* downloader = nullptr;
	LoadRes loadRes;

public:
	SteamWorkshopTool();
	virtual ~SteamWorkshopTool();
	virtual void pRun() override;
	//void initPicture();
	virtual QWidget* createMainUI() override { return new SWTUI(); }

signals:

public slots:
	void clearCacheProcess();
	void clearCacheChoice();

	void updateModProcess();
	void addModProcess(ListWidgetItemWidget* item, const ModDataTable& mod, SWTUI::ListWay way);

	void loadListProcess(const QStringList& list, SWTUI::ListWay way);

	void subscriptionProcess(bool isSubscription, const QString& id, SWTUI::ListWay way);

	void modAnalyticFinishedProcess(const QVector<ModDataTable*>& mods);
	void modAnalyticErrorProcess(QNetworkReply::NetworkError errorData);


	void messageBoxButtonProcess(const QObject* receiver, qint64 id, QPushButton* pushButton);
};