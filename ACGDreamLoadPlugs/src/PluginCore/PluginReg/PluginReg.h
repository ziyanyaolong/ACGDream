#pragma once

#include <QObject>
#include <QDir>
#include <QList>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include <QVector>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QSharedPointer>
#include <QException>
#include <QByteArray>
#include <QDataStream>
#include <QReadWriteLock>
#include <QMetaType>

#include "../../ACGDreamDevelopmentLibrary/JsonOperation/JsonOperation.h"

#include "../../PluginCalInterface/PluginCalInterface.h"

#include "../../PluginData/PluginMetaData/PluginMetaData.h"

class PluginReg : public QObject
{
	Q_OBJECT

public:
	enum class ReturnFTE
	{
		MFALSE,
		MTRUE,
		MERROR
	};

	enum class ErrorType
	{
		ReadAllFile,
		Load,
		Unload,
		File,
		JsonAnalytic
	};

	enum class ErrorList
	{
		Normal,
		FilePathEmpty,
		DirPathEmpty,
		LoadFail,
		NoName,
		EmptyPointer,
		NoPluginData,
		RepeatedLoading,
		NoMetadata,
		OpenFileError,
		NoAnalytic,
		Unknown
	};

	enum class LoadError
	{
		Normal,
		EmptyPluginLoader,
		RepeatedAdd,
		Unknown
	};

	enum class UnloadError
	{
		Normal,
		EmptyPluginLoader,
		RepeatedAdd,
		NoLoading,
		NoInLoadedList,
		NoRegister,
		UnloadingFail,
		Unknown
	};

	typedef QList<PluginMetaData*> PreList;
	typedef QMap<QString, PreList> PreMapList;
	typedef QMap<QString, PluginMetaData*> PluginLoaded;
	typedef QMap<QString, PluginMetaData*> PluginMap;

	QStringList readAllFile(const QString& dirPath);

	void loadPlugin(const QString& dirPath, const QString& fileName);
	void loadAllPlugins(const QString& dirPath);

	void unloadPlugin(const QString& name, PluginMetaData* pluginMetaData = nullptr);
	void unloadAllPlugins();

	explicit PluginReg(QObject* parent = Q_NULLPTR);
	virtual ~PluginReg();

private:
	friend class PluginCore;

	PreMapList pluginPreLoadList; //预加载队列
	PreMapList pluginPreUnloadList; //预卸载队列
	PluginLoaded pluginLoaded; //已加载队列

	QList<QString> analyticTable;
	QString name;

	void preLoaderTest(const QString& name, PluginMetaData* pluginMetaData);
	void preUnloaderTest(QPluginLoader* pluginLoader, bool flag);

	PluginCalInterface* pluginLoading(PluginMetaData* pluginMetaData);
	ReturnFTE pluginUnloading(PluginMetaData* pluginMetaData);

	PluginCalInterface* wakeUpPreLoader(PluginMetaData* pluginMetaData);
	ReturnFTE wakeUpPreUnloader(PluginMetaData* pluginMetaData);

signals:
	void loading(const QString& name);
	void loaded(const QString& name, PluginMetaData* pluginMetaData);
	void loadError(ErrorList loadError, PluginMetaData* pluginMetaData);

	void unloading(const QString& name);
	void unloaded(const QString& name, PluginMetaData* pluginMetaData);
	void unloadError(UnloadError unloadError, PluginMetaData* pluginMetaData);

	//void regPluginMainUI(PluginCalInterface* plugin);

	void errorReport(ErrorType type, ErrorList error);
	void initCompletePlugin(PluginMetaData* pluginMetaData, PluginCalInterface* plugin);
	
};
