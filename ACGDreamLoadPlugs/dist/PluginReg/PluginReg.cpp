#include "PluginReg.h"

PluginReg::PluginReg(QObject *parent)
	: QObject(parent)
{
	thread = QSharedPointer<QThread>(new QThread(this));
	thread->start();
}

PluginReg::~PluginReg()
{
	unloadAllPlugins();
	thread->quit();
	thread->wait();
}

void PluginReg::unloadAllPlugins()
{
	pluginJsonList.clear();
	for (auto plugin = plugins.begin(); plugin != plugins.end(); plugin++)
	{
		if (plugin.value())
		{
			plugin.value()->deleteLater();
			plugin.value() = nullptr;
		}
	}
	plugins.clear();
}

bool PluginReg::loadPlugin(const QString& filePath)
{
	QPluginLoader pluginLoader(filePath);
	QString name(pluginLoader.fileName());
	name = name.mid(name.lastIndexOf("/") + QString("/").size(), name.indexOf(".dll") - name.lastIndexOf("/") - QString("/").size());
	emit this->loading(name);
	PluginCalInterface* plugin = qobject_cast<PluginCalInterface*>(pluginLoader.instance());
	PluginReg::connect(plugin, &PluginCalInterface::addGui, this, &PluginReg::addGuiSignal, Qt::QueuedConnection);
	if (!pluginLoader.isLoaded())
	{
		qDebug() << filePath << __FUNCTION__ << pluginLoader.errorString();
		return false;
	}else if (plugin) {
		if (plugins.count(name) <= 0)
		{
			plugin->setParent(nullptr);
			plugin->moveToThread(thread.get());
			plugins[name] = plugin;
			pluginJsonList[plugin] = pluginLoader.metaData();
			emit plugin->pRun();
		}
		else
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
		return false;
}