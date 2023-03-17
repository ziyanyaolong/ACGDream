#include "PluginMetaData.h"

PluginMetaData::PluginMetaData()
{
	qRegisterMetaType<PluginMetaData>("PluginMetaData");
	qRegisterMetaType<PluginMetaData>("PluginMetaData&");
}

PluginMetaData::~PluginMetaData()
{
	if (pluginLoader->isLoaded())
	{
		pluginLoader->unload();
	}
	pluginLoader->deleteLater();
	pluginLoader = nullptr;
	dependencyList.clear();
	moduleList.clear();
}

bool PluginMetaData::isAddressable()
{
	return *atomic;
}