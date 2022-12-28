#include "PluginReg.h"

PluginReg::PluginReg(QObject *parent)
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


bool PluginReg::loadPlugin(const QString& filePath)
{
	QPluginLoader* pluginLoader = new QPluginLoader(filePath);
	QString name(pluginLoader->fileName());
	name = name.mid(name.lastIndexOf("/") + QString("/").size(), name.indexOf(".dll") - name.lastIndexOf("/") - QString("/").size());
	emit this->loading(name);
	PluginCalInterface* plugin = qobject_cast<PluginCalInterface*>(pluginLoader->instance());
	PluginReg::connect(plugin, &PluginCalInterface::regMainUI, this, &PluginReg::regMainUIProcess, Qt::QueuedConnection);
	if (!pluginLoader->isLoaded())
	{
		this->loadError(filePath, pluginLoader->errorString());
		return false;
	}else if (plugin) {
		if (plugins.count(name) <= 0)
		{
			plugin->setParent(this);
			plugins[name] = pluginLoader;
			plugin->pRun();
		}
		else
		{
			return false;
		}
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

	emit this->addUISignal(mainWidget);
}