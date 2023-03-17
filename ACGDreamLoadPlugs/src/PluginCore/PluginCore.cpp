#include "PluginCore.h"

PluginCore::PluginCore(QObject *parent)
	: QObject(parent)
{
	pluginReg = new PluginReg(this);

	connect(this, &PluginCore::loadAllPlugins, pluginReg, &PluginReg::loadAllPlugins);
	connect(this, &PluginCore::unloadAllPlugins, pluginReg, &PluginReg::unloadAllPlugins);
	connect(this, &PluginCore::loadPlugin, pluginReg, &PluginReg::loadPlugin);
	connect(this, &PluginCore::unloadPlugin, pluginReg, &PluginReg::unloadPlugin);

	connect(pluginReg, &PluginReg::initCompletePlugin, this, &PluginCore::runPlugin);
	connect(pluginReg, &PluginReg::loaded, this, &PluginCore::loaded);
	connect(pluginReg, &PluginReg::unloaded, this, &PluginCore::unloaded);
}

PluginCore::~PluginCore()
{
	if (pluginReg)
	{
		pluginReg->deleteLater();
		pluginReg = nullptr;
	}
}

void PluginCore::runPlugin(PluginMetaData* pluginMetaData, PluginCalInterface* plugin)
{
	connect(plugin, &PluginCalInterface::regMainUIS, this, &PluginCore::regUISignal);
	connect(this, &PluginCore::backPluginMainUI, plugin, &PluginCalInterface::backPluginMainUI, Qt::QueuedConnection);
	connect(plugin, &PluginCalInterface::removeMainUI, this, &PluginCore::removeUISignal);
	plugin->pRun();
}