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

void PluginReg::preLoaderTest(const QString& name, PluginMetadata* pluginMetadata, bool flag)
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

				if ((*j)->tablePoint != PluginMetadata::TablePoint::InPreLoadTable)
				{
					j++;
					continue;
				}

				(*j)->dependencyNeedCount--;

				if ((*j)->dependencyNeedCount > 0)
				{
					(*j)->dependencyList.push_back(pluginMetadata);
					continue;
				}

				//添加到队列
				list.push_back((*j)->name);

				//唤醒插件
				this->wakeUpPreLoader(*j);

				(*j)->tablePoint = PluginMetadata::TablePoint::InLoadedTable;

				pluginMetadata->moduleList.push_back(*j);

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

				if ((*j)->tablePoint != PluginMetadata::TablePoint::InPreUnloadTable)
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

				(*j)->tablePoint = PluginMetadata::TablePoint::InUnloadedTable;

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

PluginCalInterface* PluginReg::pluginLoading(PluginMetadata* pluginMetadata)
{
	if (pluginMetadata->pluginLoader == nullptr)
	{
		return nullptr;
	}
	else if (pluginMetadata->pluginLoader->isLoaded())
	{
		return nullptr;
	}
	else if (!(pluginMetadata->pluginLoader->load()))
	{
		this->loadError(ErrorList::LoadFail, pluginMetadata);
		return nullptr;
	}

	PluginCalInterface* plugin = nullptr;

	QT_TRY
	{
		plugin = qobject_cast<PluginCalInterface*>(pluginMetadata->pluginLoader->instance());
	}
		QT_CATCH(QException e)
	{
		this->loadError(ErrorList::Unknown, pluginMetadata);
		return plugin;
	}
	QT_CATCH(...)
	{
		this->loadError(ErrorList::Unknown, pluginMetadata);
		return plugin;
	}

	if (plugin == nullptr)
	{
		//connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
		//	emit this->regPluginMainUI((PluginCalInterface*)(sender()));
		//	});

		//connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);
		this->loadError(ErrorList::EmptyPointer, pluginMetadata);
		return plugin;
	}

	plugin->pluginMetadata = pluginMetadata;

	return plugin;
}

PluginReg::ReturnFTE PluginReg::pluginUnloading(PluginMetadata* pluginMetadata)
{
	if (pluginMetadata->pluginLoader == nullptr)
	{
		emit this->unloadError(UnloadError::EmptyPluginLoader, pluginMetadata);
		return ReturnFTE::ERROR;
	}
	else if (!(pluginMetadata->pluginLoader->isLoaded()))
	{
		emit this->unloadError(UnloadError::NoLoading, pluginMetadata);
		return ReturnFTE::ERROR;
	}
	else if (!(pluginMetadata->pluginLoader->unload()))
	{
		emit this->unloadError(UnloadError::UnloadingFail, pluginMetadata);
		return ReturnFTE::ERROR;
	}

	return ReturnFTE::TRUE;
}

PluginCalInterface* PluginReg::wakeUpPreLoader(PluginMetadata* pluginMetadata)
{
	return this->pluginLoading(pluginMetadata);
}

PluginReg::ReturnFTE PluginReg::wakeUpPreUnloader(PluginMetadata* pluginMetadata)
{
	return this->pluginUnloading(pluginMetadata);
}

void PluginReg::readAllFile(const QString& dirPath)
{
	QDir pluginsDir(dirPath);

	if (!pluginsDir.exists())
	{
		return;
	}

	QStringList filters;

	filters.append("*.dll");
	pluginsDir.setNameFilters(filters);

	emit this->backReadAllFile(dirPath, pluginsDir.entryList(QDir::Files));
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

	auto jsonValue = jsonMap.find("Name");

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
	else if ((pluginPreLoadList.find(pluginName) == pluginPreLoadList.end()) || (pluginLoaded.find(pluginName) == pluginLoaded.end()))
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::RepeatedLoading, nullptr);
		return;
	}

	PluginMetadata* pluginMetadata = new PluginMetadata();
	pluginMetadata->name = pluginName;
	pluginMetadata->pluginLoader = pluginLoader;

	//发射正在加载中的信号
	emit this->loading(fileName);

	//获取是否有依赖需求
	jsonValue = jsonMap.find("Developers");

	if (jsonValue != jsonMap.end() && (jsonValue->type() == QVariant::ByteArray))
	{
		//读取依赖列表
		QByteArray developerByteArray = jsonValue->toByteArray();
		QDataStream dataStream(developerByteArray);
		QList<QString> developerList;

		while (!dataStream.atEnd())
		{
			dataStream >> developerList;
		}

		pluginMetadata->dependencyCount = developerByteArray.size();
		pluginMetadata->dependencyNeedCount = developerByteArray.size();

		PreMapList::iterator pplIt;

		//加入到预加载列表
		for (auto i = developerList.begin(); i != developerList.end(); i++)
		{
			pplIt = pluginPreLoadList.find(*i);

			if (pplIt == pluginPreLoadList.end())
			{
				PreList preList;
				pplIt = pluginPreLoadList.insert(*i, preList);
			}

			pplIt->push_back(pluginMetadata);
		}
	}
	else
	{
		auto plugin = this->pluginLoading(pluginMetadata);

		//读取完成
		emit this->loaded(pluginName, pluginMetadata, false);
		emit this->initCompletePlugin(pluginMetadata, plugin);
	}
}

void PluginReg::loadAllPlugins(const QString& dirPath, const QStringList& fileNames)
{
	unloadAllPlugins();

	foreach(QString fileName, fileNames)
	{
		this->loadPlugin(dirPath, fileName);
	}
}

void PluginReg::unloadPlugin(const QString& name, PluginMetadata* pluginMetadata)
{
	PluginMetadata* pl = pluginMetadata;

	//对于可选参数pluginLoader检测是否在注册表中注册
	if (pl == nullptr)
	{
		auto nameIt = pluginLoaded.find(name);
		if (nameIt == pluginLoaded.end())
		{
			emit this->unloadError(UnloadError::NoRegister, pluginMetadata);
			return;
		}

		if (nameIt.value() == nullptr)
		{
			emit this->unloadError(UnloadError::NoRegister, pluginMetadata);
			return;
		}

		pl = nameIt.value();
	}

	emit this->unloading(pl->name);

	if (pl->pluginLoader)
	{
		emit this->unloadError(UnloadError::EmptyPluginLoader, pl);
		return;
	}
	else if (pl->dependencyCount != 0)
	{
		if (pl->tablePoint == PluginMetadata::TablePoint::InPreUnloadTable)
		{
			emit this->unloadError(UnloadError::RepeatedAdd, pl);
			return;
		}

		pl->tablePoint = PluginMetadata::TablePoint::InPreUnloadTable;

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
	else if (pl->pluginLoader->unload())
	{
		emit this->unloadError(UnloadError::UnloadingFail, pl);
		return;
	}

	emit this->unloaded(pl->name, pl);
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