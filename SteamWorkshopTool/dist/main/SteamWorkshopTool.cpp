#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	gui = new SWTGUI();
	modAnalytic = new ModAnalytic(this);
	database = new DataBase(this);

	qRegisterMetaType<ModDataTable>("ModDataTable");

	loadRes.allInit();

	connect(gui, &SWTGUI::clearCache, this, [&]() {
		auto steamGet = SteamGet::instance();
		QDir dir(steamGet->getData("Dirs.Caches"));
		if (!dir.exists() || dir.isEmpty())
			return;

		QDirIterator dirIterator(steamGet->getData("Dirs.Caches"), QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
		while (dirIterator.hasNext())
		{
			if (!dir.remove(dirIterator.next()))
			{
				if (dirIterator.filePath().isEmpty())
					continue;
				if (!QDir(dirIterator.filePath()).removeRecursively())
					QMetaObject::invokeMethod(gui,
						"messageBox",
						Qt::QueuedConnection,
						Q_ARG(SWTGUI::Form, SWTGUI::Form::Warning),
						Q_ARG(QString, "警告"),
						Q_ARG(QString, "文件或文件夹无法删除，请查看是否被占用"));
			}
		}
		}, Qt::QueuedConnection);
	
	connect(gui, &SWTGUI::addModReturn, this, [=](ListWidgetItemWidget* item, const ModDataTable& mod, SWTGUI::ListWay way) {
		
		item->readWidget()->setTitle(mod.title);
		item->readWidget()->setId(QString(mod.appid + mod.id));
		item->readWidget()->setSubscription(mod.isSubscribe);
		auto steamGet = SteamGet::instance();
		if (mod.image != "")
		{
			QPixmap pixmap0;
			if (pixmap0.load(QString(steamGet->getData("Dirs.Images") + "/" + mod.appid + "_" + mod.id + ".png")))
			{
				item->readWidget()->setImage(pixmap0);
			}
			else
			{
				QDir dir(steamGet->getData("Dirs.Images"));
				if (!dir.exists())
					throw "No 'Dirs.Images' dir.";

				WebCrawler* webCrawler = new WebCrawler(this);
				WebCrawler* webCrawlerImage = new WebCrawler(this);
				switch (way)
				{
				case SWTGUI::ListWay::Website:
					connect(gui, &SWTGUI::clearWebsiteList, webCrawler, &WebCrawler::deleteLater, Qt::QueuedConnection);
					connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
						auto tSteamGet = SteamGet::instance();
						const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
						WebCrawler* web = static_cast<WebCrawler*>(sender());
						QPixmap temp;
						if (!temp.loadFromData(data))
							qDebug() << "error loadFromData!";
						if (!temp.save(tSteamGet->getData("Dirs.Images") + "/" + tMod->appid + "_" + tMod->id + ".png"))
							qDebug() << "error save Pixmap!" << (tSteamGet->getData("Dirs.Images") + "/" + tMod->appid + "_" + tMod->id + ".png");
						auto* tItem = static_cast<ListWidgetItemWidget*>(web->otherData[1]);
						if ((tMod == nullptr) || (tItem == nullptr))
							return;
						tItem->readWidget()->setImage(temp);
						delete tMod;
						web->deleteLater();
						});
					break;

				case SWTGUI::ListWay::Local:
					connect(gui, &SWTGUI::clearLocalList, webCrawler, &WebCrawler::deleteLater, Qt::QueuedConnection);
					connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
						auto tSteamGet = SteamGet::instance();
						const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
						WebCrawler* web = static_cast<WebCrawler*>(sender());
						QPixmap temp;
						if (!temp.loadFromData(data))
							qDebug() << "error loadFromData!";
						if (!temp.save(tSteamGet->getData("Dirs.Images") + "/" + tMod->appid + "_" + tMod->id + ".png"))
							qDebug() << "error save Pixmap!" << (tSteamGet->getData("Dirs.Images") + "/" + tMod->appid + "_" + tMod->id + ".png");
						auto* tItem = static_cast<ListWidgetItemWidget*>(web->otherData[1]);
						if ((tMod == nullptr) || (tItem == nullptr))
							return;
						tItem->readWidget()->setImage(temp);
						delete tMod;
						web->deleteLater();
						});
					break;

				default:
					break;
				}
				webCrawler->otherData.push_back(new ModDataTable(mod));
				webCrawler->otherData.push_back(item);
				webCrawler->websiteLink(mod.image);
			}
		}
		}, Qt::QueuedConnection);

	connect(gui, &SWTGUI::updateMod, this, [&]() {
		database->open();
		qDebug() << database->readModDataTableKeyAll();
		database->close();
		}, Qt::QueuedConnection);

	connect(gui, &SWTGUI::loadList, this, [&](const QStringList& list, SWTGUI::ListWay way) {
		SteamGet* getTemp = SteamGet::instance();
		QList<ModDataTable> modList;
		database->open();
		switch (way)
		{
		case SWTGUI::ListWay::Website:
			emit modAnalytic->analyticMods(getTemp->getData("SteamWorkShop.BaseHttp") + list[0] + getTemp->getData("SteamWorkShop.Search") + list[1] + getTemp->getData("SteamWorkShop.Page") + list[2]);
			break;

		case SWTGUI::ListWay::Local:
			database->updata(DataBase::Events::All);
			QMetaObject::invokeMethod(gui,
				"clearModList",
				Qt::QueuedConnection,
				Q_ARG(SWTGUI::ListWay, SWTGUI::ListWay::Local)
			);
			modList = database->readModDataTableAll();
			foreach (auto i, modList)
			{
				QMetaObject::invokeMethod(gui,
					"addMod",
					Qt::QueuedConnection,
					Q_ARG(const ModDataTable&, i),
					Q_ARG(SWTGUI::ListWay, SWTGUI::ListWay::Local)
				);
			}
			break;

		default:
			break;
		}
		database->close();
		}, Qt::QueuedConnection); 

	connect(gui, &SWTGUI::subscription, this, [&](bool isSubscription, const QString& id, SWTGUI::ListWay way) {
		ModDataTable* temp = nullptr;
		ModDataTable mod;
		database->open();
		switch (way)
		{
		case SWTGUI::ListWay::Website:
			temp = modAnalytic->findMod(id);

			if (temp == nullptr)
				return;

			if (isSubscription)
			{
				temp->isSubscribe = true;
				database->addModDataTable(temp, DataBase::WriteWay::Overlay);
			}
			else
			{
				database->deleteModDataTable(id);
			}
			break;
		case SWTGUI::ListWay::Local:
			mod = database->readModDataTable(id);

			if (mod.id == "")
				return;

			if (isSubscription)
			{
				mod.isSubscribe = true;
				database->removeListEventData(id, DataBase::Events::All);
				database->addDataEvent(&mod, DataBase::WriteWay::Overlay);
			}
			else
			{
				database->removeListEventData(id, DataBase::Events::All);
				database->deleteDataEvent(id);
			}
			break;
		case SWTGUI::ListWay::All:
			break;
		default:
			break;
		}

		database->close();
		}, Qt::QueuedConnection);

	connect(modAnalytic, &ModAnalytic::finished, this, [&](const QVector<ModDataTable*>& mods) {
		database->open();
		foreach(ModDataTable * i, mods)
		{
			if (database->findModDataTable(i->appid + i->id))
			{
				i->isSubscribe = true;
			}
			QMetaObject::invokeMethod(gui,
				"addMod",
				Qt::QueuedConnection,
				Q_ARG(const ModDataTable&, *i),
				Q_ARG(SWTGUI::ListWay, SWTGUI::ListWay::Website)
				);
		}
		database->close();
		});

	connect(modAnalytic, &ModAnalytic::error, this, [&](QNetworkReply::NetworkError errorData) {
		QMetaObject::invokeMethod(gui, 
			"messageBox", 
			Qt::QueuedConnection, 
			Q_ARG(SWTGUI::Form, SWTGUI::Form::Critical), 
			Q_ARG(QString, "错误"),
			Q_ARG(QString, "网址无法访问，请查看网络是否可用"));
		});

}

SteamWorkshopTool::~SteamWorkshopTool()
{

	if (modAnalytic)
		modAnalytic->deleteLater();

	if (database)
		database->deleteLater();

	modAnalytic = nullptr;
	database = nullptr;
}

void SteamWorkshopTool::pRun()
{
	emit this->addGui(gui);
}