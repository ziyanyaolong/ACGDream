#include "PluginReg.h"

PluginReg::PluginReg(QObject* parent)
	: QObject(parent)
{
}

PluginReg::~PluginReg()
{
	unloadAllPlugins();
}

void PluginReg::unloadAllPlugins()
{
	for (auto plugin = plugins.begin(); plugin != plugins.end(); plugin++)
	{
		if (plugin.value())
		{
			if (plugin.value()->isLoaded())
			{
				if ((plugin.value()->unload()) == false)
				{
					this->loadError(plugin.value()->objectName(), QString("无法卸载,其他实例正在使用此插件!"));
				}
			}
			plugin.value()->deleteLater();
		}
	}
	plugins.clear();
}

bool PluginReg::unloadPlugin(const QString& pluginName)
{
	auto tempPlugin = plugins.find(pluginName);
	if (tempPlugin != plugins.end())
	{
		tempPlugin.value()->unload();
		tempPlugin.value()->deleteLater();
		plugins.erase(tempPlugin);

		return true;
	}
	return false;
}


bool PluginReg::loadPlugin(const QString& filePath)
{
	QPluginLoader* pluginLoader = new QPluginLoader(filePath, this);
	QString name(pluginLoader->fileName());
	name = name.mid(name.lastIndexOf("/") + QString("/").size(), name.indexOf(".dll") - name.lastIndexOf("/") - QString("/").size());

	if (plugins.find(name) != plugins.end())
	{
		this->loadError(filePath, QString("已加载此插件,请勿重复加载!"));
		pluginLoader->deleteLater();
		return false;
	}

	emit this->loading(name);

	PluginCalInterface* plugin = nullptr;

	QT_TRY
	{
		plugin = qobject_cast<PluginCalInterface*>(pluginLoader->instance());
	}
	QT_CATCH(QException e)
	{
		this->loadError(filePath, QString(e.what()));
		return false;
	}
	QT_CATCH(...)
	{
		this->loadError(filePath, pluginLoader->errorString());
		return false;
	}

	if (!pluginLoader->isLoaded())
	{
		this->loadError(filePath, pluginLoader->errorString());
		return false;
	}
	else if (plugin) {
		plugin->setPluginLoader(pluginLoader);

		connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
			emit this->regPluginMainUI((PluginCalInterface*)(sender()));
			});

		connect(plugin, &PluginCalInterface::deleteMainUI, this, &PluginReg::removeUISignal);

		plugins[name] = pluginLoader;

		QT_TRY
		{
			plugin->pRun();
		}
			QT_CATCH(QException e)
		{
			this->loadError(filePath, QString(e.what()));
			return false;
		}
		QT_CATCH(...)
		{
			this->loadError(filePath, pluginLoader->errorString());
			return false;
		}
	}
	else
	{
		this->loadError(filePath, QString("插件指针为空,请检查插件是否有问题!"));
		return false;
	}

	

	return true;
}

bool PluginReg::loadAllPlugins(const QString& dirPath)
{
	unloadAllPlugins();
	QDir pluginsDir(dirPath);

	QStringList filters;
	filters.append("*.dll");
	pluginsDir.setNameFilters(filters);

	bool isLoad = false;

	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		if (loadPlugin(dirPath + "/" + fileName))
		{
			isLoad = true;
		}
	}

	if (isLoad)
	{
		return true;
	}
	else
	{
		this->loadError("Error", "Could not load the plugin");
		return false;
	}
}

void PluginReg::backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget)
{
	auto tempPluginLoader = (plugin->getPluginLoader());

	emit plugin->backPluginMainUI(mainWidget);
}