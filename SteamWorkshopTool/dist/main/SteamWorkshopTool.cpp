#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	//QString itemHead = QString("<div class=\"workshopItem\">");
	//QString itemTail = QString("</script>");
	//QString itemJsHead = QString("SharedFileBindMouseHover(");
	//QString itemJsTail = QString(");");
	//QString itemImageHead1 = QString("<img class=\"");
	//QString itemImageHead2 = QString("src=\"");
	//QString itemImageTail = QString("\">");
	gui = new SWTGUI();
	modAnalytic = new ModAnalytic(this);
	database = new DataBase(this);
	QFile file(":/SteamWorkshopTool/assets/Config/AnalyticTable.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "errorOpen!";
		return;
	}
	JsonOperation json(file.readAll(), this);
	SteamGet::instance()->addData(json.analyticAll());
	
	connect(gui, &SWTGUI::pushButtonResponse_WebAddress, this, [&](const QString& appid, const QString& page) {
		SteamGet* getTemp = SteamGet::instance();
		emit modAnalytic->analyticMods(getTemp->findData("SteamWorkShop.BaseHttp") + appid + getTemp->findData("SteamWorkShop.Page") + page);
		}, Qt::QueuedConnection); 
	connect(gui, &SWTGUI::pushButtonResponse_Previous, this, [&](const QString& appid, const QString& page) {
		SteamGet* getTemp = SteamGet::instance();
		emit modAnalytic->analyticMods(getTemp->findData("SteamWorkShop.BaseHttp") + appid + getTemp->findData("SteamWorkShop.Page") + page);
		}, Qt::QueuedConnection);

	connect(gui, &SWTGUI::pushButtonResponse_Next, this, [&](const QString& appid, const QString& page) {
		SteamGet* getTemp = SteamGet::instance();
		emit modAnalytic->analyticMods(getTemp->findData("SteamWorkShop.BaseHttp") + appid + getTemp->findData("SteamWorkShop.Page") + page);
		}, Qt::QueuedConnection);

	connect(gui, &SWTGUI::pushButtonResponse_Subscription, this, [&](bool isSubscription, const QString& id) {
		ModDataTable* temp = modAnalytic->findMod(id);

		if (temp == nullptr)
		{
			qDebug() << id;
			return;
		}

		if (isSubscription)
		{
			auto temp = modAnalytic->findMod(id);
			temp->isSubscribe = true;
			emit this->signalDataBass_save(temp, DataBase::Way::Overlay);
		}
		else
		{
			auto temp = modAnalytic->findMod(id);
			temp->isSubscribe = false;
			emit this->signalDataBass_delete(id);
		}
		}, Qt::QueuedConnection);

	connect(modAnalytic, &ModAnalytic::finish, gui, &SWTGUI::clearModList, Qt::QueuedConnection);
	
	connect(modAnalytic, &ModAnalytic::finished, this, [&](const QVector<ModDataTable*>& mods) {
		foreach(ModDataTable * i, mods)
		{
			if (database->findModDataTable(i->appid + i->id))
			{
				i->isSubscribe = true;
			}
			emit this->signalGui_addMod(*i);
		}
		});

	connect(this, &SteamWorkshopTool::signalGui_addMod, gui, &SWTGUI::addMod, Qt::QueuedConnection);
	connect(this, &SteamWorkshopTool::signalDataBass_delete, database, &DataBase::deleteModDataTable, Qt::QueuedConnection);
	connect(this, &SteamWorkshopTool::signalDataBass_save, database, &DataBase::addModDataTable, Qt::QueuedConnection);

}

SteamWorkshopTool::~SteamWorkshopTool()
{

	if (modAnalytic)
		modAnalytic->deleteLater();

	if (database)
		database->deleteLater();

	
}

void SteamWorkshopTool::pRun()
{
	emit this->addGui(gui);
}