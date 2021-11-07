#include "PluginReg.h"

PluginReg::PluginReg(QObject *parent)
	: QObject(parent)
{
	thread = QSharedPointer<QThread>(new QThread(this));
	thread->start();
}

PluginReg::~PluginReg()
{
	foreach(PluginCalInterface *plugin, plugins)
	{
		if (plugin)
			plugin->deleteLater();
	}
	thread->quit();
	thread->wait();
}

bool PluginReg::loadPlugin(const QString& filePath)
{
	QPluginLoader pluginLoader(filePath);
	QObject* plugin = pluginLoader.instance();
	if (!pluginLoader.isLoaded())
	{
		qDebug() << filePath << __FUNCTION__ << pluginLoader.errorString();
		return false;
	}else if (plugin) {
		auto pluginClass = qobject_cast<PluginCalInterface*>(plugin);
		if (plugins.indexOf(pluginClass) == -1)
		{
			pluginClass->setParent(nullptr);
			pluginClass->moveToThread(thread.get());
			plugins.push_back(pluginClass);
		}
		else
			return false;
	}

	return true;
}

bool PluginReg::loadPluginAll(const QString& dirPath)
{
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