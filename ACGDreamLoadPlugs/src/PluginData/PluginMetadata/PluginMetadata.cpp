#include "PluginMetadata.h"

PluginMetadata::PluginMetadata()
{
}

PluginMetadata::~PluginMetadata()
{
}

bool PluginMetadata::isAddressable()
{
	return *atomic;
}