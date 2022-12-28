#include "PluginReg.h"

PluginReg::PluginReg(QObject *parent)
	: QObject(parent)
{
	connect(this, &PluginReg::loadAllPlugins, this, &PluginReg::loadAllPluginsProcess);
	connect(this, &PluginReg::unloadAllPlugins, this, &PluginReg::unloadAllPluginsProcess);
	connect(this, &PluginReg::loadPlugin, this, &PluginReg::loadPluginProcess);
	connect(this, &PluginReg::unloadPlugin, this, &PluginReg::unloadPluginProcess);
}

PluginReg::~PluginReg()
{
	unloadAllPluginsProcess();
}

void PluginReg::unloadAllPluginsProcess()
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

bool PluginReg::unloadPluginProcess(const QString& pluginName)
{
	auto tempPlugin = plugins.find(pluginName);
	if (tempPlugin != plugins.end())
	{
		auto tempPluginWidget = pluginWidgetList.find(tempPlugin.value());
		if (tempPluginWidget != pluginWidgetList.end())
		{
			emit this->removeUISignal(tempPluginWidget.value());
			pluginWidgetList.erase(tempPluginWidget);
		}

		tempPlugin.value()->unload();
		tempPlugin.value()->deleteLater();
		plugins.erase(tempPlugin);

		return true;
	}
	return false;
}


bool PluginReg::loadPluginProcess(const QString& filePath)
{
	QPluginLoader* pluginLoader = new QPluginLoader(filePath, this);
	QString name(pluginLoader->fileName());
	name = name.mid(name.lastIndexOf("/") + QString("/").size(), name.indexOf(".dll") - name.lastIndexOf("/") - QString("/").size());
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

	connect(plugin, &PluginCalInterface::regMainUIS, this, [&]() {
		emit this->regPluginMainUI((PluginCalInterface*)(sender()));
		});

	connect(this, &PluginReg::backPluginMainUI, plugin, &PluginCalInterface::backPluginMainUI);

	if (!pluginLoader->isLoaded())
	{
		this->loadError(filePath, pluginLoader->errorString());
		return false;
	}else if (plugin) {
		if (plugins.count(name) <= 0)
		{
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

bool PluginReg::loadAllPluginsProcess(const QString& dirPath)
{
	unloadAllPluginsProcess();
	QDir pluginsDir(dirPath);

	QStringList filters;
	filters.append("*.dll");
	pluginsDir.setNameFilters(filters);

	bool isLoad = false;

	foreach(QString fileName, pluginsDir.entryList(QDir::Files))
	{
		if (loadPluginProcess(dirPath + "/" + fileName))
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

void PluginReg::regMainUIProcess(QWidget* mainWidget)
{
	auto tempSender = (const QPluginLoader*)(sender());

	auto tempPluginWidget = pluginWidgetList.find(tempSender);

	if (tempPluginWidget == pluginWidgetList.end())
	{
		pluginWidgetList[tempSender] = mainWidget;
	}
	else
	{
		if (tempPluginWidget.value() != nullptr)
		{
			tempPluginWidget.value() = mainWidget;
		}
		tempPluginWidget.value() = mainWidget;
	}
}