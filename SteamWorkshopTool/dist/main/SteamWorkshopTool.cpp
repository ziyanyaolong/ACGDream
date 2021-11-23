#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	gui = new SWTGUI();
	modAnalytic = new ModAnalytic(this);
	database = new DataBase(this);

	//�����ҳ��������
	QFile file(":/SteamWorkshopTool/assets/Config/AnalyticTable.json");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "errorOpen!";
		return;
	}
	JsonOperation json(file.readAll(), this);
	SteamGet::instance()->setParent(this);
	SteamGet::instance()->addData(json.analyticAll());

	connect(gui, &SWTGUI::clearCache, this, [&]() {
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
					QMetaObject::invokeMethod(gui,
						"messageBox",
						Qt::QueuedConnection,
						Q_ARG(SWTGUI::Form, SWTGUI::Form::Warning),
						Q_ARG(QString, "����"),
						Q_ARG(QString, "�ļ����ļ����޷�ɾ������鿴�Ƿ�ռ��"));
			}
		}
		}, Qt::QueuedConnection);
	
	connect(gui, &SWTGUI::updateMod, this, [&]() {
		qDebug() << database->readModDataTableAll();
		}, Qt::QueuedConnection);

	connect(gui, &SWTGUI::loadList, this, [&](const QStringList& list, SWTGUI::ListWay way) {
		SteamGet* getTemp = SteamGet::instance();
		switch (way)
		{
		case SWTGUI::ListWay::Website:
			emit modAnalytic->analyticMods(getTemp->findData("SteamWorkShop.BaseHttp") + list[0] + getTemp->findData("SteamWorkShop.Search") + list[1] + getTemp->findData("SteamWorkShop.Page") + list[2]);
			break;

		case SWTGUI::ListWay::Local:

			break;

		default:
			break;
		}
		
		}, Qt::QueuedConnection); 

	connect(gui, &SWTGUI::subscription, this, [&](bool isSubscription, const QString& id) {
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
			emit database->addModDataTable(temp, DataBase::Way::Overlay);
		}
		else
		{
			auto temp = modAnalytic->findMod(id);
			temp->isSubscribe = false;
			emit database->deleteModDataTable(id);
		}
		}, Qt::QueuedConnection);

	connect(modAnalytic, &ModAnalytic::finished, this, [&](const QVector<ModDataTable*>& mods) {
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
		});

	connect(modAnalytic, &ModAnalytic::error, this, [&](QNetworkReply::NetworkError errorData) {
		QMetaObject::invokeMethod(gui, 
			"messageBox", 
			Qt::QueuedConnection, 
			Q_ARG(SWTGUI::Form, SWTGUI::Form::Critical), 
			Q_ARG(QString, "����"),
			Q_ARG(QString, "��ַ�޷����ʣ���鿴�����Ƿ����"));
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