#pragma once

#include <QObject>

class ACGDream;
class PluginCalInterface
{
public:
	PluginCalInterface() {}
	virtual ~PluginCalInterface() {}
public:
	virtual int run(int, char* [], ACGDream const*) = 0;
};

#define PluginCalInterface_iid "Examples.Plugin.PluginCalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(PluginCalInterface, PluginCalInterface_iid)
QT_END_NAMESPACE