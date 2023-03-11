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
	connect(this, &PluginCore::backPluginMainUI, this, &PluginCore::setPluginMainUI);
}

PluginCore::~PluginCore()
{
	if (pluginReg)
	{
		pluginReg->deleteLater();
		pluginReg = nullptr;
	}
}

void PluginCore::runPlugin(PluginMetadata* pluginMetadata, PluginCalInterface* plugin)
{
	plugin->pRun();
}

void PluginCore::setPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget)
{
	plugin->mainUI = mainWidget;
}