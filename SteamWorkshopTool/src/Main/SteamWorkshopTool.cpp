#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	: PluginCalInterface()
{
	modAnalytic = new ModAnalytic(this);
	database = new DataBase(this);
	downloader = new Downloader(this);

	qRegisterMetaType<ModDataTable>("ModDataTable");

	loadRes.allInit();

	SteamGet::instance()->setParent(this);
}

SteamWorkshopTool::~SteamWorkshopTool()
{
	ui = nullptr;

	if (downloader)
	{
		downloader->deleteLater();
	}

	downloader = nullptr;

	if (modAnalytic)
	{
		modAnalytic->deleteLater();
	}

	modAnalytic = nullptr;

	if (database)
	{
		database->deleteLater();
	}

	database = nullptr;

	SteamGet::deleteSteamGet();
}

void SteamWorkshopTool::pRun()
{
	emit this->regMainUI(&createMUI);

	this->ui = static_cast<SWTUI*>(this->getMainUI());

	connect(ui, &SWTUI::messageBoxButtonConnect, this, &SteamWorkshopTool::messageBoxButtonProcess);


	connect(this->ui, &SWTUI::clearCache, this, &SteamWorkshopTool::clearCacheChoice);

	connect(ui, &SWTUI::addModReturn, this, &SteamWorkshopTool::addModProcess);

	connect(ui, &SWTUI::updateMod, this, &SteamWorkshopTool::updateModProcess);

	connect(ui, &SWTUI::loadList, this, &SteamWorkshopTool::loadListProcess);

	connect(ui, &SWTUI::subscription, this, &SteamWorkshopTool::subscriptionProcess);

	connect(modAnalytic, &ModAnalytic::finished, this, &SteamWorkshopTool::modAnalyticFinishedProcess);

	connect(modAnalytic, &ModAnalytic::error, this, &SteamWorkshopTool::modAnalyticErrorProcess);

	connect(ui, &SWTUI::startSteamCMD, this, [&]() {
		downloader->init();
		});
}

void SteamWorkshopTool::clearCacheProcess()
{
	QDir dir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
	if (!dir.exists() || dir.isEmpty())
		return;

	QDirIterator dirIterator(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches", QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);
	while (dirIterator.hasNext())
	{
		if (!dir.remove(dirIterator.next()))
		{
			if (dirIterator.filePath().isEmpty())
				continue;
			if (!QDir(dirIterator.filePath()).removeRecursively())
			{
				QMetaObject::invokeMethod(ui,
					"messageBox",
					Qt::QueuedConnection,
					Q_ARG(SWTUI::Form, SWTUI::Form::Warning),
					Q_ARG(QString, "警告"),
					Q_ARG(QString, "文件或文件夹无法删除,请查看是否被占用"));
			}
		}
	}
	QMetaObject::invokeMethod(ui,
		"messageBox",
		Qt::QueuedConnection,
		Q_ARG(QString, QString::fromLocal8Bit("信息")),
		Q_ARG(QString, QString(QString::fromLocal8Bit("成功删除所有缓存文件!"))),
		Q_ARG(QMessageBox::Icon, QMessageBox::Icon::Information));
}

void SteamWorkshopTool::messageBoxButtonProcess(const QObject* receiver, qint64 id, QPushButton* pushButton)
{
	if ((qint64)(receiver) == (qint64)(this))
	{
		switch (id)
		{
		case 0:
			connect(pushButton, &QPushButton::clicked, this, &SteamWorkshopTool::clearCacheProcess);
			break;
		case 1:
			//取消按键处理，留位，不处理
			break;

		default:
			break;
		}
	}
}

void SteamWorkshopTool::clearCacheChoice()
{
	auto steamGet = SteamGet::instance();
	QVector<SWTUI::QMessageBoxButtonData> list;
	list.push_back(SWTUI::QMessageBoxButtonData(QString("确定"), QMessageBox::ButtonRole::AcceptRole, true, this));
	list.push_back(SWTUI::QMessageBoxButtonData(QString("取消"), QMessageBox::ButtonRole::RejectRole, false, this));
	QMetaObject::invokeMethod(this->ui,
		"messageBox",
		Qt::QueuedConnection,
		Q_ARG(const QString&, QString::fromLocal8Bit("警告")),
		Q_ARG(const QString&, QString(QString::fromLocal8Bit("是否删除:") + QString(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + QString::fromLocal8Bit(" 下的所有文件？"))),
		Q_ARG(QMessageBox::Icon, QMessageBox::Icon::Warning),
		Q_ARG(QVector<SWTUI::QMessageBoxButtonData>, list));
}

void SteamWorkshopTool::addModProcess(ListWidgetItemWidget* item, const ModDataTable& mod, SWTUI::ListWay way)
{
	//item->readItem()->setSizeHint(QSize(256, 256));
	item->readWidget()->setTitle(mod.title);
	item->readWidget()->setId(QString(mod.appid + mod.id));
	item->readWidget()->setSubscription(mod.isSubscribe);

	if (mod.image != "")
	{
		QPixmap pixmap0;
		if (pixmap0.load(QString((QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + "/" + mod.appid + "_" + mod.id + ".png")))
		{
			item->readWidget()->setImage(pixmap0);
		}
		else
		{
			QDir dir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
			if (!dir.exists())
			{
				if (!dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches"))
				{
					throw "Dir error!";
				}
			}

			WebCrawler* webCrawler = new WebCrawler(this);
			WebCrawler* webCrawlerImage = new WebCrawler(this);
			switch (way)
			{
			case SWTUI::ListWay::Website:
				connect(ui, &SWTUI::clearWebsiteList, webCrawler, &WebCrawler::deleteLater, Qt::QueuedConnection);
				connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
					auto tSteamGet = SteamGet::instance();
					const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
					WebCrawler* web = static_cast<WebCrawler*>(sender());
					QPixmap temp;
					if (!temp.loadFromData(data))
						qDebug() << "error loadFromData!";
					if (!temp.save((QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + "/" + tMod->appid + "_" + tMod->id + ".png"))
						qDebug() << "error save Pixmap!" << ((QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + "/" + tMod->appid + "_" + tMod->id + ".png");
					auto* tItem = static_cast<ListWidgetItemWidget*>(web->otherData[1]);
					if ((tMod == nullptr) || (tItem == nullptr))
						return;
					tItem->readWidget()->setImage(temp);
					delete tMod;
					web->deleteLater();
					});
				break;

			case SWTUI::ListWay::Local:
				connect(ui, &SWTUI::clearLocalList, webCrawler, &WebCrawler::deleteLater, Qt::QueuedConnection);
				connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
					auto tSteamGet = SteamGet::instance();
					const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
					WebCrawler* web = static_cast<WebCrawler*>(sender());
					QPixmap temp;
					if (!temp.loadFromData(data))
						qDebug() << "error loadFromData!";
					if (!temp.save((QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + "/" + tMod->appid + "_" + tMod->id + ".png"))
						qDebug() << "error save Pixmap!" << ((QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches") + "/" + tMod->appid + "_" + tMod->id + ".png");
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
}

void SteamWorkshopTool::updateModProcess()
{
	database->open();
	qDebug() << database->readModDataTableKeyAll();
	database->close();
}

void SteamWorkshopTool::loadListProcess(const QStringList& list, SWTUI::ListWay way)
{
	SteamGet* getTemp = SteamGet::instance();
	QList<ModDataTable> modList;
	database->open();
	switch (way)
	{
	case SWTUI::ListWay::Website:
		emit modAnalytic->analyticMods(getTemp->getData("SteamWorkShop.BaseHttp") + list[0] + getTemp->getData("SteamWorkShop.Search") + list[1] + getTemp->getData("SteamWorkShop.Page") + list[2]);
		break;

	case SWTUI::ListWay::Local:
		database->updata(DataBase::Events::All);
		QMetaObject::invokeMethod(ui,
			"clearModList",
			Qt::QueuedConnection,
			Q_ARG(SWTUI::ListWay, SWTUI::ListWay::Local)
		);
		modList = database->readModDataTableAll();
		foreach(auto i, modList)
		{
			QMetaObject::invokeMethod(ui,
				"addMod",
				Qt::QueuedConnection,
				Q_ARG(const ModDataTable&, i),
				Q_ARG(SWTUI::ListWay, SWTUI::ListWay::Local)
			);
		}
		break;

	default:
		break;
	}
	database->close();
}

void SteamWorkshopTool::subscriptionProcess(bool isSubscription, const QString& id, SWTUI::ListWay way)
{
	ModDataTable* temp = nullptr;
	ModDataTable mod;
	database->open();
	switch (way)
	{
	case SWTUI::ListWay::Website:
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
	case SWTUI::ListWay::Local:
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
	case SWTUI::ListWay::All:
		break;
	default:
		break;
	}

	database->close();
}

void SteamWorkshopTool::modAnalyticFinishedProcess(const QVector<ModDataTable*>& mods)
{
	database->open();
	foreach(ModDataTable * i, mods)
	{
		if (database->findModDataTable(i->appid + i->id))
		{
			i->isSubscribe = true;
		}
		QMetaObject::invokeMethod(ui,
			"addMod",
			Qt::QueuedConnection,
			Q_ARG(const ModDataTable&, *i),
			Q_ARG(SWTUI::ListWay, SWTUI::ListWay::Website)
		);
	}
	database->close();
}

void SteamWorkshopTool::modAnalyticErrorProcess(QNetworkReply::NetworkError errorData)
{
	qDebug() << errorData;
	QMetaObject::invokeMethod(ui,
		"messageBox",
		Qt::QueuedConnection,
		Q_ARG(QString, "错误"),
		Q_ARG(QString, "网址无法访问，请查看网络是否可用"),
		Q_ARG(QMessageBox::Icon, QMessageBox::Icon::Critical));
}
