#pragma once

#include <QObject>
#include <QString>
#include <QPluginLoader>
#include <QMutex>
#include <QList>

class PluginMetadata
{
public:
	PluginMetadata();
	virtual ~PluginMetadata();

	enum class TablePoint
	{
		None = 0x0,
		InPreLoadTable,
		InPreUnloadTable,
		InLoadedTable,
		InUnloadedTable,
		Unknown
	};

	QPluginLoader* pluginLoader = nullptr;
	QString name = "";

	quint32 dependencyCount = 0;
	quint32 dependencyNeedCount = 0;

	QList<const PluginMetadata*> dependencyList;
	QList<const PluginMetadata*> moduleList;

	quint32 threadID = 0;
	TablePoint tablePoint = TablePoint::None;

	bool isAddressable();

private:
	QAtomicInteger<bool>* atomic = nullptr;
};

