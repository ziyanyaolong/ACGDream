#include "PluginReg.h"

PluginReg::PluginReg(QObject* parent)
	: QObject(parent)
{
	connect(this, &PluginReg::loadError, this, [&](ErrorList error) {
		emit this->errorReport(ErrorType::Load, error);
		});

	connect(this, &PluginReg::loaded, this, &PluginReg::preLoaderTest);
	connect(this, &PluginReg::unloaded, this, &PluginReg::preUnloaderTest);

	QDir dir(QCoreApplication::applicationDirPath() + "/Config");

	if (dir.exists())
	{
		dir.mkdir("ACGDreamLoadPlugs");
	}

	dir.cd("ACGDreamLoadPlugs");
	QFile file(dir.path() + "/" + "PluginAnalytic");

	if (dir.isEmpty())
	{
		if (!file.open(QIODevice::WriteOnly))
		{
			file.errorString();
			this->errorReport(ErrorType::File, ErrorList::OpenFileError);
			return;
		}
	}

	QJsonParseError jsonParserError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(file.fileName().toLocal8Bit(), &jsonParserError);
	QJsonObject jsonObject = jsonDocument.object();

	if (!jsonDocument.isObject())
	{
		this->errorReport(ErrorType::JsonAnalytic, ErrorList::NoAnalytic);
		return;
	}

	if (!jsonObject.contains("AnalyticTable"))
	{
		this->errorReport(ErrorType::JsonAnalytic, ErrorList::NoAnalytic);
		return;
	}

	auto variantList = jsonObject.value("AnalyticTable").toArray().toVariantList();

	for (auto i = variantList.begin(); i != variantList.end(); i++)
	{
		analyticTable.push_back(i->toString());
	}

	analyticTable.sort();
}

PluginReg::~PluginReg()
{
	unloadAllPlugins();
}

void PluginReg::preLoaderTest(const QString& name, PluginMetaData* pluginMetaData)
{
	if (pluginPreLoadList.isEmpty())
	{
		return;
	}

	auto it0 = pluginPreLoadList.find(name);

	if (it0 == pluginPreLoadList.end())
	{
		return;
	}

	QList<PluginMetaData*> wakeList;

	for (auto it1 = it0.value().begin(); it1 != it0.value().end(); it1++)
	{
		((*it1)->dependencyNeedCount)--;

		if ((*it1)->dependencyNeedCount > 0)
		{
			continue;
		}

		wakeList.push_back(*it1);
	}

	if (wakeList.isEmpty())
	{
		return;
	}

	for (auto it1 = wakeList.begin(); it1 != wakeList.end(); it1++)
	{
		this->wakeUpPreLoader(*it1);
	}
}

void PluginReg::preUnloaderTest(const QString& name, PluginMetaData* pluginMetaData)
{
	if (pluginPreUnloadList.isEmpty())
	{
		return;
	}

	auto it0 = pluginPreUnloadList.find(name);

	if (it0 == pluginPreUnloadList.end())
	{
		return;
	}

	QList<PluginMetaData*> wakeList;

	for (auto it1 = it0.value().begin(); it1 != it0.value().end(); it1++)
	{
		((*it1)->dependencyNeedCount)--;

		if ((*it1)->dependencyNeedCount > 0)
		{
			continue;
		}

		wakeList.push_back(*it1);
	}

	if (wakeList.isEmpty())
	{
		return;
	}

	for (auto it1 = wakeList.begin(); it1 != wakeList.end(); it1++)
	{
		this->wakeUpPreUnloader(*it1);
	}
}

PluginCalInterface* PluginReg::pluginLoading(PluginMetaData* pluginMetaData)
{
	if (pluginMetaData->pluginLoader == nullptr)
	{
		return nullptr;
	}
	else if (pluginMetaData->pluginLoader->isLoaded())
	{
		return nullptr;
	}
	else if (!(pluginMetaData->pluginLoader->load()))
	{
		this->loadError(ErrorList::LoadFail, pluginMetaData);
		return nullptr;
	}

	PluginCalInterface* plugin = nullptr;

	QT_TRY
	{
		plugin = qobject_cast<PluginCalInterface*>(pluginMetaData->pluginLoader->instance());
	}
	QT_CATCH(QException e)
	{
		this->loadError(ErrorList::Unknown, pluginMetaData);
		return plugin;
	}
	QT_CATCH(...)
	{
		this->loadError(ErrorList::Unknown, pluginMetaData);
		return plugin;
	}

	if (plugin == nullptr)
	{
		this->loadError(ErrorList::EmptyPointer, pluginMetaData);
		return plugin;
	}

	plugin->pluginMetaData = pluginMetaData;

	this->casePluginList(PluginMetaData::TablePoint::InLoadedTable, pluginMetaData);

	emit this->loaded(pluginMetaData->name, pluginMetaData);
	emit this->initCompletePlugin(pluginMetaData, plugin);

	return plugin;
}

PluginReg::ReturnFTE PluginReg::pluginUnloading(PluginMetaData* pluginMetaData)
{
	ReturnFTE fte = ReturnFTE::MFALSE;

	if (pluginMetaData->pluginLoader == nullptr)
	{
		emit this->unloadError(UnloadError::EmptyPluginLoader, pluginMetaData);
		fte = ReturnFTE::MERROR;
	}
	else if (!(pluginMetaData->pluginLoader->isLoaded()))
	{
		emit this->unloadError(UnloadError::NoLoading, pluginMetaData);
		fte = ReturnFTE::MERROR;
	}
	else if (!(pluginMetaData->pluginLoader->unload()))
	{
		emit this->unloadError(UnloadError::UnloadingFail, pluginMetaData);
		fte = ReturnFTE::MERROR;
		return fte;
	}

	this->casePluginList(PluginMetaData::TablePoint::InUnloadedTable, pluginMetaData);

	emit this->unloaded(pluginMetaData->name, pluginMetaData);

	delete pluginMetaData;
	pluginMetaData = nullptr;

	fte = ReturnFTE::MTRUE;

	if (pluginLoaded.isEmpty() && pluginPreLoadList.isEmpty() && pluginPreUnloadList.isEmpty())
	{
		pluginUnloaded.clear();
	}

	return fte;
}

PluginCalInterface* PluginReg::wakeUpPreLoader(PluginMetaData* pluginMetaData)
{
	return this->pluginLoading(pluginMetaData);
}

PluginReg::ReturnFTE PluginReg::wakeUpPreUnloader(PluginMetaData* pluginMetaData)
{
	return this->pluginUnloading(pluginMetaData);
}

PluginReg::ReturnFTE PluginReg::casePluginList(PluginMetaData::TablePoint tablePoint, PluginMetaData* pluginMetaData, void* pointer)
{
	ReturnFTE fte = ReturnFTE::MFALSE;

	if (this->casePluginListErase(pluginMetaData, pointer) != ReturnFTE::MTRUE)
	{
		fte = ReturnFTE::MERROR;
	}
	else if (this->casePluginListPush(tablePoint, pluginMetaData, pointer) != ReturnFTE::MTRUE)
	{
		fte = ReturnFTE::MERROR;
	}
	else
	{
		fte = ReturnFTE::MTRUE;
	}

	return fte;
}

PluginReg::ReturnFTE PluginReg::casePluginListErase(PluginMetaData* pluginMetaData, void* pointer)
{
	if (pluginMetaData == nullptr)
	{
		return ReturnFTE::MERROR;
	}

	ReturnFTE fte = ReturnFTE::MFALSE;

	switch (pluginMetaData->tablePoint)
	{
	case PluginMetaData::TablePoint::None:
		pluginMetaData->tablePoint = PluginMetaData::TablePoint::None;

		fte = ReturnFTE::MTRUE;
		break;

	case PluginMetaData::TablePoint::InPreLoadTable:
	{
		if (pluginPreLoadList.isEmpty())
		{
			fte = ReturnFTE::MERROR;
			break;
		}

		QStringList tempList;

		if (pointer)
		{
			tempList = (*(reinterpret_cast<QStringList*>(pointer)));
		}
		else
		{
			tempList = pluginMetaData->dependencyList;
		}

		for (auto it0 = tempList.begin(); it0 != tempList.end(); it0++)
		{
			auto it1 = pluginPreLoadList.find(*it0);

			if (it1 == pluginPreLoadList.end())
			{
				continue;
			}

			auto it2 = qFind(it1.value().begin(), it1.value().end(), pluginMetaData);

			if (it2 == it1.value().end())
			{
				continue;
			}

			it1.value().erase(it2);

			if (it1.value().isEmpty())
			{
				pluginPreLoadList.erase(it1);
			}
		}

		tempList.clear();

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::None;

		fte = ReturnFTE::MTRUE;
	}
	break;

	case PluginMetaData::TablePoint::InPreUnloadTable:
	{
		if (pluginPreUnloadList.isEmpty())
		{
			fte = ReturnFTE::MERROR;
			break;
		}

		QStringList tempList;

		if (pointer)
		{
			tempList = (*(reinterpret_cast<QStringList*>(pointer)));
		}
		else
		{
			tempList = pluginMetaData->dependencyList;
		}

		for (auto it0 = tempList.begin(); it0 != tempList.end(); it0++)
		{
			auto it1 = pluginPreUnloadList.find(*it0);

			if (it1 == pluginPreUnloadList.end())
			{
				continue;
			}

			auto it2 = qFind(it1.value().begin(), it1.value().end(), pluginMetaData);

			if (it2 == it1.value().end())
			{
				continue;
			}

			it1.value().erase(it2);

			if (it1.value().isEmpty())
			{
				pluginPreUnloadList.erase(it1);
			}
		}

		tempList.clear();

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::None;

		fte = ReturnFTE::MTRUE;
	}
	break;

	case PluginMetaData::TablePoint::InLoadedTable:
	{
		QString pluginName = pluginMetaData->name;

		if (pluginName.isEmpty())
		{
			fte = ReturnFTE::MERROR;
			break;
		}

		PluginReg::PluginMap::iterator it0;

		if (pointer == nullptr)
		{
			it0 = pluginLoaded.find(pluginName);
		}
		else
		{
			it0 = (*(reinterpret_cast<PluginReg::PluginMap::iterator*>(pointer)));
		}


		if (it0 == pluginLoaded.end())
		{
			fte = ReturnFTE::MERROR;
			break;
		}
		else if ((*it0) == nullptr)
		{
			pluginLoaded.erase(it0);
			fte = ReturnFTE::MFALSE;
			break;
		}

		pluginLoaded.erase(it0);

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::None;

		fte = ReturnFTE::MTRUE;
	}
	break;

	case PluginMetaData::TablePoint::InUnloadedTable:
		pluginMetaData->tablePoint = PluginMetaData::TablePoint::None;

		fte = ReturnFTE::MTRUE;
		break;

	case PluginMetaData::TablePoint::Unknown:
		fte = ReturnFTE::MERROR;
		break;

	default:
		fte = ReturnFTE::MERROR;
		break;
	}

	return fte;
}

PluginReg::ReturnFTE PluginReg::casePluginListPush(PluginMetaData::TablePoint tablePoint, PluginMetaData* pluginMetaData, void* pointer)
{
	if (pluginMetaData->tablePoint != PluginMetaData::TablePoint::None)
	{
		return ReturnFTE::MFALSE;
	}

	ReturnFTE fte = ReturnFTE::MFALSE;
	switch (tablePoint)
	{
	case PluginMetaData::TablePoint::None:
		fte = ReturnFTE::MTRUE;
		break;

	case PluginMetaData::TablePoint::InPreLoadTable:
	{
		if (pointer == nullptr)
		{
			fte = ReturnFTE::MERROR;
			break;
		}

		auto pDList = reinterpret_cast<QStringList*>(pointer);
		for (auto it0 = pDList->begin(); it0 != pDList->end(); it0++)
		{
			auto it1 = pluginPreLoadList.find(*it0);

			if (it1 == pluginPreLoadList.end())
			{
				PreList preList;
				preList.push_back(pluginMetaData);
				pluginPreLoadList.insert(*it0, preList);
			}
			else
			{
				it1.value().push_back(pluginMetaData);
			}
		}

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::InPreLoadTable;

		fte = ReturnFTE::MTRUE;
	}
	break;

	case PluginMetaData::TablePoint::InPreUnloadTable:
	{
		if (pointer == nullptr)
		{
			fte = ReturnFTE::MERROR;
			break;
		}

		auto pDList = reinterpret_cast<QStringList*>(pointer);
		for (auto it0 = pDList->begin(); it0 != pDList->end(); it0++)
		{
			auto it1 = pluginPreUnloadList.find(*it0);

			if (it1 == pluginPreUnloadList.end())
			{
				PreList preList;
				preList.push_back(pluginMetaData);
				pluginPreUnloadList.insert(*it0, preList);
			}
			else
			{
				it1.value().push_back(pluginMetaData);
			}
		}

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::InPreUnloadTable;

		fte = ReturnFTE::MTRUE;
	}
	break;

	case PluginMetaData::TablePoint::InLoadedTable:
		pluginLoaded.insert(pluginMetaData->name, pluginMetaData);

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::InLoadedTable;

		fte = ReturnFTE::MTRUE;
		break;

	case PluginMetaData::TablePoint::InUnloadedTable:
		pluginUnloaded.push_back(pluginMetaData->name);

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::InUnloadedTable;

		fte = ReturnFTE::MTRUE;
		break;

	case PluginMetaData::TablePoint::Unknown:
		fte = ReturnFTE::MERROR;
		break;

	default:
		fte = ReturnFTE::MERROR;
		break;
	}

	return fte;
}

PluginReg::ReturnFTE PluginReg::sortPluginList()
{
	return ReturnFTE::MTRUE;
}

QStringList PluginReg::readAllFile(const QString& dirPath)
{
	QDir pluginsDir(dirPath);

	if (!pluginsDir.exists())
	{
		return QStringList();
	}

	QStringList filters;

	filters.append("*.dll");
	pluginsDir.setNameFilters(filters);

	return pluginsDir.entryList(QDir::Files);
}

void PluginReg::loadPlugin(const QString& dirPath, const QString& fileName)
{
	if (dirPath.isEmpty())
	{
		emit this->loadError(ErrorList::DirPathEmpty, nullptr);
		return;
	}
	else if (fileName.isEmpty())
	{
		emit this->loadError(ErrorList::FilePathEmpty, nullptr);
		return;
	}

	QPluginLoader* pluginLoader = new QPluginLoader(dirPath + QString("/") + fileName, this);

	//获取插件Json配置
	ACGDREAM_DL::JsonOperation json;
	auto jsonData = pluginLoader->metaData();
	json.setJsonObject(&jsonData);

	QVariantMap jsonMap = json.analyticAll();

	if (jsonMap.isEmpty())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetadata, nullptr);
		return;
	}

	auto jsonValue = jsonMap.find("MetaData.PluginName");

	if (jsonValue == jsonMap.end())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetadata, nullptr);
		return;
	}
	else if (jsonValue.value() == QString(""))
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetadata, nullptr);
		return;
	}

	QString pluginName = jsonValue.value().toString();

	if (pluginName.isEmpty())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetadata,  nullptr);
		return;
	}
	else if (pluginLoaded.find(pluginName) != pluginLoaded.end())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::RepeatedLoading, nullptr);
		return;
	}

	PluginMetaData* pluginMetaData = new PluginMetaData();
	pluginMetaData->name = pluginName;
	pluginMetaData->pluginLoader = pluginLoader;

	//发射正在加载中的信号
	emit this->loading(fileName);

	//获取是否有依赖需求
	jsonValue = jsonMap.find("MetaData.Dependencys");

	QStringList developerList;

	if (jsonValue != jsonMap.end())
	{
		//读取依赖列表
		if (jsonValue->type() == QVariant::List)
		{
			auto qvList = jsonValue->toList();

			for (auto i = qvList.begin(); i != qvList.end(); i++)
			{
				auto name = i->toString();
				if (name.isEmpty())
				{
					continue;
				}
				developerList.push_back(i->toString());
			}
		}
		else
		{
			auto name = jsonValue.value().toString();
			if (!(name.isEmpty()))
			{
				developerList.push_back(name);
			}
		}
	}

	if (!(developerList.isEmpty()))
	{
		pluginMetaData->dependencyList = developerList;
		pluginMetaData->dependencyCount = developerList.size();
		pluginMetaData->dependencyNeedCount = developerList.size();

		for (auto it0 = developerList.begin(); it0 != developerList.end(); it0++)
		{
			auto it1 = pluginLoaded.find(*(it0));

			if (it1 == pluginLoaded.end())
			{
				continue;
			}

			(pluginMetaData->dependencyNeedCount)--;
		}

		if (pluginMetaData->dependencyNeedCount > 0)
		{
			this->casePluginList(PluginMetaData::TablePoint::InPreLoadTable, pluginMetaData);
		}
		else
		{
			this->casePluginList(PluginMetaData::TablePoint::InLoadedTable, pluginMetaData);
			auto plugin = this->pluginLoading(pluginMetaData);
		}

		developerList.clear();
	}
	else
	{
		auto plugin = this->pluginLoading(pluginMetaData);
	}
}

void PluginReg::loadAllPlugins(const QString& dirPath)
{
	unloadAllPlugins();

	QStringList fileNames = readAllFile(dirPath);

	for (auto it = fileNames.begin(); it != fileNames.end(); it++)
	{
		this->loadPlugin(dirPath, *it);
	}
}

void PluginReg::unloadPlugin(const QString& name, PluginMetaData* pluginMetaData)
{
	PluginMetaData* pl = pluginMetaData;

	//对于可选参数pluginLoader检测是否在注册表中注册
	if (pl == nullptr)
	{
		auto nameIt = pluginLoaded.find(name);
		if (nameIt == pluginLoaded.end())
		{
			emit this->unloadError(UnloadError::NoRegister, pluginMetaData);
			return;
		}

		if (nameIt.value() == nullptr)
		{
			emit this->unloadError(UnloadError::NoRegister, pluginMetaData);
			return;
		}

		pl = nameIt.value();
	}

	emit this->unloading(pl->name);

	if (pl->pluginLoader == nullptr)
	{
		emit this->unloadError(UnloadError::EmptyPluginLoader, pl);
		return;
	}
	else if (pl->dependencyCount != 0)
	{
		if (pl->tablePoint == PluginMetaData::TablePoint::InPreUnloadTable)
		{
			emit this->unloadError(UnloadError::RepeatedAdd, pl);
			return;
		}

		pl->dependencyNeedCount = pl->dependencyCount;

		QStringList tempList = pl->dependencyList;

		for (auto it = tempList.begin(); it != tempList.end(); it++)
		{
			auto plIt = pluginUnloaded.indexOf(*it);
			
			if (plIt != -1)
			{
				(pl->dependencyNeedCount)--;
			}

			if (pl->dependencyNeedCount > 0)
			{
				switch (this->casePluginList(PluginMetaData::TablePoint::InPreUnloadTable, pluginMetaData))
				{
				case PluginReg::ReturnFTE::MFALSE:
					break;
				case PluginReg::ReturnFTE::MTRUE:
					break;
				case PluginReg::ReturnFTE::MERROR:
					break;
				default:
					break;
				}
			}
			else
			{
				switch (this->pluginUnloading(pl))
				{
				case PluginReg::ReturnFTE::MFALSE:
					break;
				case PluginReg::ReturnFTE::MTRUE:
					break;
				case PluginReg::ReturnFTE::MERROR:
					break;
				default:
					break;
				}
			}
		}

		return;
	}
	else
	{
		emit this->pluginUnloading(pl);
	}
}

void PluginReg::unloadAllPlugins()
{
	//清理预加载插件列表
	if (!(pluginPreLoadList.isEmpty()))
	{
		for (auto it0 = pluginPreLoadList.begin(); it0 != pluginPreLoadList.end(); it0++)
		{
			for (auto it1 = it0.value().begin(); it1 != it0.value().end(); it1++)
			{
				delete (*it1);
				(*it1) = nullptr;
			}
			it0.value().clear();
		}
		pluginPreLoadList.clear();
	}

	if (!(pluginPreUnloadList.isEmpty()))
	{
		for (auto it0 = pluginPreUnloadList.begin(); it0 != pluginPreUnloadList.end(); it0++)
		{
			for (auto it1 = it0.value().begin(); it1 != it0.value().end(); it1++)
			{
				delete (*it1);
				(*it1) = nullptr;
			}
			it0.value().clear();
		}
		pluginPreUnloadList.clear();
	}

	auto it = pluginLoaded.begin();
	while (it != pluginLoaded.end())
	{
		if (it.value() == nullptr)
		{
			it = pluginLoaded.erase(it);
			continue;
		}

		this->unloadPlugin(it.key(), it.value());
		if (!(pluginLoaded.isEmpty()))
		{
			it = pluginLoaded.begin();
		}
		else
		{
			it = pluginLoaded.end();
		}
	}
}

//void PluginReg::backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget)
//{
//	auto tempPluginLoader = (plugin->getPluginLoader());
//
//	plugin->backPluginMainUI(mainWidget);
//}