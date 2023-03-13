#include "PluginMetaData.h"

PluginMetaData::PluginMetaData()
{
	qRegisterMetaType<PluginMetaData>("PluginMetaData");
	qRegisterMetaType<PluginMetaData>("PluginMetaData&");
}

PluginMetaData::~PluginMetaData()
{
}

bool PluginMetaData::isAddressable()
{
	return *atomic;
}