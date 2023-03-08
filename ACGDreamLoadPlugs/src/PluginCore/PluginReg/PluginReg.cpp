#include "PluginReg.h"

PluginReg::PluginReg(QObject* parent)
	: QObject(parent)
{
	connect(this, &PluginReg::loadError, this, [&](ErrorList error) {
		emit this->errorReport(ErrorType::Load, error);
		});

	connect(this, &PluginReg::loaded, this, &PluginReg::preloaderTest);

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

PluginCalInterface* PluginReg::pluginInit(QPluginLoader* pluginLoader)
{
	if (!pluginLoader->isLoaded())
	{
		if (pluginLoader->load())
		{
			this->loadError(ErrorList::LoadFail, name, "", pluginLoader);
			return nullptr;
		}
	}

	PluginCalInterface* plugin = nullptr;

	QT_TRY
	{
		plugin = qobject_cast<PluginCalInterface*>(pluginLoader->instance());
	}
		QT_CATCH(QException e)
	{
		this->loadError(ErrorList::Unknown, name, QString(e.what()), pluginLoader);
		return plugin;
	}
	QT_CATCH(...)
	{
		this->loadError(ErrorList::Unknown, name, pluginLoader->errorString(), pluginLoader);
		return plugin;
	}

	if (plugin == nullptr)
	{
		//connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
		//	emit this->regPluginMainUI((PluginCalInterface*)(sender()));
		//	});

		//connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);
		this->loadError(ErrorList::EmptyPointer, name, pluginLoader->errorString(), pluginLoader);
		return plugin;
	}

	plugin->pluginLoader = pluginLoader;

	return plugin;
}

void PluginReg::preloaderTest(const QString& name, QPluginLoader* pluginLoader, bool flag)
{
	if (!flag)
	{
		return;
	}

	QList<QString> list;
	list.push_back(name);

	QList<QString>::iterator listIt = list.begin();

	//加载插件(如果插件被加载则再次检测是否有其附属)
	for (auto i = pluginPreloadList.begin(); i != pluginPreloadList.end();)
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

				(*j)->dependencyNeedCount--;

				if ((*j)->dependencyNeedCount > 0)
				{
					continue;
				}

				list.push_back((*j)->name);

				auto plugin = this->wakeUpPreloader((*j)->pluginLoader);

				if (plugin)
				{
					plugin->dependencyCount = (*j)->dependencyCount;
					plugin->name = (*j)->name;
					plugin->accessoryModuleList.push_back((*j)->pluginLoader);
				}

				delete (*j);
				(*j) = nullptr;

				j = i->erase(j);
			}

			//检测如果所有附属全部加载完成,则删除此键值
			if (i->isEmpty())
			{
				//这里不需要把迭代器后面的值传给i,因为后面要么直接赋值为begin,要么直接结束
				pluginPreloadList.erase(i);
			}

			if (listIt != list.end())
			{
				listIt++;
				i = pluginPreloadList.begin();
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

PluginCalInterface* PluginReg::wakeUpPreloader(QPluginLoader* pluginLoader)
{
	return this->pluginInit(pluginLoader);
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

void PluginReg::unloadPlugin(const QString& name, QPluginLoader* pluginLoader)
{
	QPluginLoader* pl = pluginLoader;

	//对于可选参数pluginLoader检测是否在注册表中注册
	if (pluginLoader == nullptr)
	{
		//auto nameIt = plugins.find(name);
		//if (nameIt == plugins.end())
		//{
		//	emit this->unloadError(UnloadError::NoRegister, name, pluginLoader);
		//	return;
		//}

		if (nameIt.value() == nullptr)
		{
			emit this->unloadError(UnloadError::NoRegister, name, pluginLoader);
			return;
		}

		pl = nameIt.value();
	}

	if (!pl->isLoaded())
	{
		emit this->unloadError(UnloadError::NoLoading, name, pl);
		return;
	}

	//预加载列表是否需要检测？需要优化
}

//bool PluginReg::unloadPluginsProcess(QPluginLoader* pluginLoader)
//{
//	if (pluginLoader->isLoaded())
//	{
//		auto plugin = (qobject_cast<PluginCalInterface*>(pluginLoader->instance()));
//
//		if (plugin == nullptr)
//		{
//			this->loadError(ErrorList::EmptyPointer, plugin->objectName(), pluginLoader);
//			pluginLoader->deleteLater();
//			return false;
//		}
//
//		auto pluginAccessoryModuleList = plugin->getAccessoryModuleList();
//
//		if (pluginAccessoryModuleList != nullptr)
//		{
//			for (auto pluginAM = pluginAccessoryModuleList->begin(); pluginAM != pluginAccessoryModuleList->end(); pluginAM++)
//			{
//				this->unloadPluginsProcess(*pluginAM);
//			}
//		}
//
//		pluginAccessoryModuleList->clear();
//		delete pluginAccessoryModuleList;
//		pluginAccessoryModuleList = nullptr;
//
//		if ((pluginLoader->unload()) == false)
//		{
//			this->loadError(pluginLoader->objectName(), QString("卸载失败,其他实例正在使用此插件!"));
//			pluginLoader->deleteLater();
//			return false;
//		}
//	}
//	else
//	{
//		this->loadError(pluginLoader->objectName(), QString("卸载失败,插件实例未运行!"));
//		pluginLoader->deleteLater();
//		return false;
//	}
//	pluginLoader->deleteLater();
//	return true;
//}
//
//void PluginReg::loadPluginsProcess0()
//{
//	foreach(auto i, plugins)
//	{
//		if (i == nullptr)
//		{
//			continue;
//		}
//		else if (!i->isLoaded())
//		{
//			PluginCalInterface* plugin = nullptr;
//
//			QT_TRY
//			{
//				plugin = qobject_cast<PluginCalInterface*>(i->instance());
//			}
//				QT_CATCH(QException e)
//			{
//				this->loadError(i->fileName(), QString(e.what()));
//				continue;
//			}
//			QT_CATCH(...)
//			{
//				this->loadError(i->fileName(), i->errorString());
//				continue;
//			}
//
//			if (plugin)
//			{
//				continue;
//			}
//
//			plugin->setPluginLoader(i);
//
//			connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
//				emit this->regPluginMainUI((PluginCalInterface*)(sender()));
//				});
//
//			connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);
//
//			QT_TRY
//			{
//				plugin->pRun();
//			}
//				QT_CATCH(QException e)
//			{
//				this->loadError(i->fileName(), QString(e.what()));
//				continue;
//			}
//			QT_CATCH(...)
//			{
//				this->loadError(i->fileName(), i->errorString());
//				continue;
//			}
//
//		}
//		else
//		{
//			continue;
//		}
//	}
//
//
//	PluginCalInterface* plugin = nullptr;
//
//	QT_TRY
//	{
//		plugin = qobject_cast<PluginCalInterface*>(pluginLoader->instance());
//	}
//		QT_CATCH(QException e)
//	{
//		this->loadError(pluginLoader->fileName(), QString(e.what()));
//		return -1;
//	}
//	QT_CATCH(...)
//	{
//		this->loadError(pluginLoader->fileName(), pluginLoader->errorString());
//		return -1;
//	}
//
//	if (!pluginLoader->isLoaded())
//	{
//		this->loadError(pluginLoader->fileName(), pluginLoader->errorString());
//		return -1;
//	}
//	else if (plugin)
//	{
//
//	}
//	else
//	{
//		this->loadError(pluginLoader->fileName(), QString("插件指针为空,请检查插件是否有问题!"));
//		return -1;
//	}
//
//	return 1;
//}
//
//void PluginReg::loadPluginsProcess1(const QString& name, QPluginLoader* pluginLoader)
//{
//	PluginCalInterface* plugin = nullptr;
//
//	QT_TRY
//	{
//		plugin = qobject_cast<PluginCalInterface*>(pluginLoader->instance());
//	}
//		QT_CATCH(QException e)
//	{
//		this->loadError(pluginLoader->fileName(), QString(e.what()));
//		return -1;
//	}
//	QT_CATCH(...)
//	{
//		this->loadError(pluginLoader->fileName(), pluginLoader->errorString());
//		return -1;
//	}
//
//	if (!pluginLoader->isLoaded())
//	{
//		this->loadError(pluginLoader->fileName(), pluginLoader->errorString());
//		return -1;
//	}
//	else if (plugin)
//	{
//		plugin->setPluginLoader(pluginLoader);
//
//		connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
//			emit this->regPluginMainUI((PluginCalInterface*)(sender()));
//			});
//
//		connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);
//
//		plugins[name] = pluginLoader;
//
//		QT_TRY
//		{
//			plugin->pRun();
//		}
//			QT_CATCH(QException e)
//		{
//			this->loadError(pluginLoader->fileName(), QString(e.what()));
//			return -1;
//		}
//		QT_CATCH(...)
//		{
//			this->loadError(pluginLoader->fileName(), pluginLoader->errorString());
//			return -1;
//		}
//	}
//	else
//	{
//		this->loadError(pluginLoader->fileName(), QString("插件指针为空,请检查插件是否有问题!"));
//		return -1;
//	}
//
//	return 1;
//}
////
//void PluginReg::unloadAllPlugins()
//{
//	for (auto plugin = plugins.begin(); plugin != plugins.end(); plugin++)
//	{
//		if (plugin.value())
//		{
//			this->unloadPluginsProcess(plugin.value());
//		}
//	}
//	plugins.clear();
//}
//
//void PluginReg::unloadPlugin(QList<QPluginLoader*>& pluginLoaderList)
//{
//	auto pluginLoader = plugins.find(pluginName);
//	if (pluginLoader != plugins.end())
//	{
//		if (pluginLoader.value())
//		{
//			this->unloadPluginsProcess(pluginLoader.value());
//			pluginLoader.value()->unload();
//			plugins.erase(pluginLoader);
//		}
//
//		return true;
//	}
//	return false;
//}
//
//void PluginReg::loadAllPlugins(const QString& dirPath, const QStringList& fileNames)
//{
//	unloadAllPlugins();
//	if (list.isEmpty())
//	{
//		emit this->loadError(ErrorList::FilePathEmpty, QString(), nullptr);
//		return;
//	}
//
//	foreach(QString fileName, list)
//	{
//		this->loadPlugin(dirPath, fileName);
//	}
//}

void PluginReg::loadPlugin(const QString& dirPath, const QString& fileName)
{
	if (dirPath.isEmpty())
	{
		emit this->loadError(ErrorList::DirPathEmpty, dirPath, QString(""), nullptr);
		return;
	}
	else if (fileName.isEmpty())
	{
		emit this->loadError(ErrorList::FilePathEmpty, fileName, QString(""), nullptr);
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
		emit this->loadError(ErrorList::NoMetaData, fileName, pluginLoader->errorString(), nullptr);
		return;
	}

	auto jsonValue = jsonMap.find("Name");

	if (jsonValue == jsonMap.end())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetaData, fileName, pluginLoader->errorString(), nullptr);
		return;
	}
	else if (jsonValue.value() == QString(""))
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetaData, fileName, pluginLoader->errorString(), nullptr);
		return;
	}

	QString pluginName = jsonValue.value().toString();

	if (pluginName.isEmpty())
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::NoMetaData, pluginName, pluginLoader->errorString(), nullptr);
		return;
	}
	else if ((pluginPreloadList.find(pluginName) == pluginPreloadList.end()) || (pluginLoaded.find(pluginName) == pluginLoaded.end()))
	{
		pluginLoader->deleteLater();
		emit this->loadError(ErrorList::RepeatedLoading, fileName, QString(""), nullptr);
		return;
	}

	//发射正在加载中的信号
	emit this->loading(fileName);

	////获取成功后加入插件列表
	//plugins.insert(pluginName, pluginLoader);

	//获取是否有依赖需求
	jsonValue = jsonMap.find("Developers");

	if (jsonValue != jsonMap.end() && jsonValue->type() == QVariant::ByteArray)
	{
		//读取依赖列表
		QByteArray developerByteArray = jsonValue->toByteArray();
		QDataStream dataStream(developerByteArray);
		QList<QString> developerList;

		while (!dataStream.atEnd())
		{
			dataStream >> developerList;
		}

		PluginDependencyData* pdd = nullptr;
		PluginMapList::iterator pplIt;

		//加入到预加载列表
		for (auto i = developerList.begin(); i != developerList.end(); i++)
		{
			pplIt = pluginPreloadList.find(*i);

			if (pplIt == pluginPreloadList.end())
			{
				PreloadList prList;
				pplIt = pluginPreloadList.insert(*i, prList);
			}

			if (pdd == nullptr)
			{
				pdd = new PluginDependencyData;
				pdd->dependencyCount = developerByteArray.size();
				pdd->pluginLoader = pluginLoader;
				pdd->name = pluginName;
				pdd->dependencyNeedCount = developerByteArray.size();
			}

			pplIt->push_back(pdd);
		}
		emit this->loaded(pluginName, pluginLoader, true);
		return;
	}

	auto plugin = this->pluginInit(pluginLoader);
	
	//读取完成
	emit this->loaded(pluginName, pluginLoader, false);
	emit this->initCompletePlugin(pluginLoader, plugin);
}

//void PluginReg::backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget)
//{
//	auto tempPluginLoader = (plugin->getPluginLoader());
//
//	plugin->backPluginMainUI(mainWidget);
//}