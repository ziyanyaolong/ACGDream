#include "PluginReg.h"

PluginReg::PluginReg(QObject* parent)
	: QObject(parent)
{
	connect(this, &PluginReg::loadError, this, [&](ErrorList error) {
		emit this->errorReport(ErrorType::Load, error);
		});

	connect(this, &PluginReg::loaded, this, &PluginReg::preLoaderTest);

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
	QList<QString> list;
	list.push_back(name);

	QList<QString>::iterator listIt = list.begin();

	//加载插件(如果插件被加载则再次检测是否有其附属)
	for (auto i = pluginPreLoadList.begin(); i != pluginPreLoadList.end();)
	{
		if (i.key() == *listIt)
		{
			for (auto j = i->begin(); j != i->end();)
			{
				//防止出现空项
				while ((*j) == nullptr)
				{
					j = i->erase(j);
					continue;
				}

				if ((*j)->tablePoint != PluginMetaData::TablePoint::InPreLoadTable)
				{
					j++;
					continue;
				}

				(*j)->dependencyNeedCount--;

				if ((*j)->dependencyNeedCount > 0)
				{
					(*j)->dependencyList.push_back(pluginMetaData);
					continue;
				}

				//添加到队列
				list.push_back((*j)->name);

				//唤醒插件
				this->wakeUpPreLoader(*j);

				(*j)->tablePoint = PluginMetaData::TablePoint::InLoadedTable;

				pluginMetaData->moduleList.push_back(*j);

				j = i->erase(j);
			}

			//检测如果所有附属全部加载完成,则删除此键值
			if (i->isEmpty())
			{
				//这里不需要把迭代器后面的值传给i,因为后面要么直接赋值为begin,要么直接结束
				pluginPreLoadList.erase(i);
			}

			if (listIt != list.end())
			{
				listIt = list.erase(listIt);
				i = pluginPreLoadList.begin();
				continue;
			}
			else
			{
				break;
			}
		}
		i++;
	}

	//用完清理,好习惯
	list.clear();
}

void PluginReg::preUnloaderTest(QPluginLoader* pluginLoader, bool flag)
{
	QList<QString> list;
	list.push_back(name);

	QList<QString>::iterator listIt = list.begin();

	//卸载插件(如果插件被卸载则再次检测是否有其附属)
	for (auto i = pluginPreUnloadList.begin(); i != pluginPreUnloadList.end();)
	{
		if (i.key() == *listIt)
		{
			for (auto j = i->begin(); j != i->end();)
			{
				while ((*j) == nullptr)
				{
					j = i->erase(j);
					continue;
				}

				if ((*j)->tablePoint != PluginMetaData::TablePoint::InPreUnloadTable)
				{
					j++;
					continue;
				}

				(*j)->dependencyNeedCount--;

				if ((*j)->dependencyNeedCount > 0)
				{
					continue;
				}

				list.push_back((*j)->name);

				this->wakeUpPreUnloader(*j);

				(*j)->tablePoint = PluginMetaData::TablePoint::InUnloadedTable;

				pluginLoader->deleteLater();

				pluginLoader = nullptr;

				delete (*j);
				(*j) = nullptr;

				j = i->erase(j);
			}

			//检测如果所有附属全部卸载完成,则删除此键值
			if (i->isEmpty())
			{
				//这里不需要把迭代器后面的值传给i,因为后面要么直接赋值为begin,要么直接结束
				pluginPreLoadList.erase(i);
			}

			if (listIt != list.end())
			{
				listIt = list.erase(listIt);
				i = pluginPreUnloadList.begin();
				continue;
			}
			else
			{
				break;
			}
		}
		i++;
	}

	//用完清理,好习惯
	list.clear();
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
		//connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
		//	emit this->regPluginMainUI((PluginCalInterface*)(sender()));
		//	});

		//connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);
		this->loadError(ErrorList::EmptyPointer, pluginMetaData);
		return plugin;
	}

	plugin->pluginMetaData = pluginMetaData;

	pluginLoaded.insert(pluginMetaData->name, pluginMetaData);

	pluginMetaData->tablePoint = PluginMetaData::TablePoint::InLoadedTable;

	emit this->loaded(pluginMetaData->name, pluginMetaData);
	emit this->initCompletePlugin(pluginMetaData, plugin);

	return plugin;
}

PluginReg::ReturnFTE PluginReg::pluginUnloading(PluginMetaData* pluginMetaData)
{
	if (pluginMetaData->pluginLoader == nullptr)
	{
		emit this->unloadError(UnloadError::EmptyPluginLoader, pluginMetaData);
		return ReturnFTE::MERROR;
	}
	else if (!(pluginMetaData->pluginLoader->isLoaded()))
	{
		emit this->unloadError(UnloadError::NoLoading, pluginMetaData);
		return ReturnFTE::MERROR;
	}
	else if (!(pluginMetaData->pluginLoader->unload()))
	{
		emit this->unloadError(UnloadError::UnloadingFail, pluginMetaData);
		return ReturnFTE::MERROR;
	}

	return ReturnFTE::MTRUE;
}

PluginCalInterface* PluginReg::wakeUpPreLoader(PluginMetaData* pluginMetaData)
{
	return this->pluginLoading(pluginMetaData);
}

PluginReg::ReturnFTE PluginReg::wakeUpPreUnloader(PluginMetaData* pluginMetaData)
{
	return this->pluginUnloading(pluginMetaData);
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

	qDebug() << jsonValue->type();

	if ((jsonValue != jsonMap.end()) && (jsonValue->type() == QVariant::List))
	{
		//读取依赖列表
		auto qvList = jsonValue->toList();
		//QByteArray developerByteArray = ;
		//QDataStream dataStream(developerByteArray);
		QList<QString> developerList;

		for (auto i = qvList.begin(); i != qvList.end(); i++)
		{
			developerList.push_back(i->toString());
		}

		//while (!dataStream.atEnd())
		//{
		//	dataStream >> developerList;
		//}

		pluginMetaData->dependencyCount = developerList.size();
		pluginMetaData->dependencyNeedCount = developerList.size();

		PreMapList::iterator pplIt = pluginPreLoadList.begin();

		//加入到预加载列表
		for (auto i = developerList.begin(); i != developerList.end(); i++)
		{
			pplIt = pluginPreLoadList.find(*i);

			if (pplIt == pluginPreLoadList.end())
			{
				PreList preList;
				pplIt = pluginPreLoadList.insert(*i, preList);
			}

			pplIt->push_back(pluginMetaData);
		}

		pluginMetaData->tablePoint = PluginMetaData::TablePoint::InPreLoadTable;
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
	qDebug() << fileNames.size();
	for (quint32 i = 0; i < fileNames.size(); i++)
	{
		qDebug() << fileNames[i];
		this->loadPlugin(dirPath, fileNames[i]);
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

		pl->tablePoint = PluginMetaData::TablePoint::InPreUnloadTable;

		pl->dependencyNeedCount = pl->dependencyCount;

		for (auto i = pl->dependencyList.begin(); i != pl->dependencyList.end(); i++)
		{
			auto ppuIt = pluginPreUnloadList.find((*i)->name);

			if (ppuIt == pluginPreUnloadList.end())
			{
				PreList preList;
				pluginPreUnloadList.insert((*i)->name, preList);
			}

			ppuIt.value().push_back(pl);
		}

		return;

	}
	else if (!(pl->pluginLoader->isLoaded()))
	{
		emit this->unloadError(UnloadError::NoLoading, pl);
		return;
	}
	else if (!(pl->pluginLoader->unload()))
	{
		emit this->unloadError(UnloadError::UnloadingFail, pl);
		return;
	}

	auto it = pluginLoaded.find(pl->name);

	if (it == pluginLoaded.end())
	{
		this->unloadError(UnloadError::NoInLoadedList, pluginMetaData);
	}
	else
	{
		pluginLoaded.erase(it);
	}

	pl->tablePoint = PluginMetaData::TablePoint::InUnloadedTable;

	emit this->unloaded(pl->name, pl);

	pl->dependencyList.clear();
	pl->moduleList.clear();
	pl->pluginLoader->deleteLater();
	delete pl;
	pl = nullptr;
}

void PluginReg::unloadAllPlugins()
{
	for (auto i = pluginLoaded.begin(); i != pluginLoaded.end(); i++)
	{
		this->unloadPlugin(i.key(), i.value());
	}
}

//void PluginReg::backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget)
//{
//	auto tempPluginLoader = (plugin->getPluginLoader());
//
//	plugin->backPluginMainUI(mainWidget);
//}