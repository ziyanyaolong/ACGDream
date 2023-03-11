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

#include "../../ACGDreamDevelopmentLibrary/JsonOperation/JsonOperation.h"

#include "../../PluginCalInterface/PluginCalInterface.h"

#include "../../PluginData/PluginMetadata/PluginMetadata.h"

class PluginReg : public QObject
{
	Q_OBJECT

public:
	enum class ReturnFTE
	{
		FALSE = 0x0,
		TRUE,
		ERROR
	};

	enum class ErrorType
	{
		ReadAllFile = 0x0,
		Load,
		Unload,
		File,
		JsonAnalytic
	};

	enum class ErrorList
	{
		Normal = 0x0,
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
		Normal = 0x0,
		EmptyPluginLoader,
		RepeatedAdd,
		Unknown
	};

	enum class UnloadError
	{
		Normal = 0x0,
		EmptyPluginLoader,
		RepeatedAdd,
		NoLoading,
		NoRegister,
		UnloadingFail,
		Unknown
	};

	typedef QList<PluginMetadata*> PreList;
	typedef QMap<QString, PreList> PreMapList;
	typedef QMap<QString, PluginMetadata*> PluginLoaded;
	typedef QMap<QString, PluginMetadata*> PluginMap;

	void readAllFile(const QString& dirPath);

	void loadPlugin(const QString& dirPath, const QString& fileName);
	void loadAllPlugins(const QString& dirPath, const QStringList& fileNames);

	void unloadPlugin(const QString& name, PluginMetadata* pluginMetadata = nullptr);
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

	void preLoaderTest(const QString& name, PluginMetadata* pluginMetadata, bool flag);
	void preUnloaderTest(QPluginLoader* pluginLoader, bool flag);

	PluginCalInterface* pluginLoading(PluginMetadata* pluginMetadata);
	ReturnFTE pluginUnloading(PluginMetadata* pluginMetadata);

	PluginCalInterface* wakeUpPreLoader(PluginMetadata* pluginMetadata);
	ReturnFTE wakeUpPreUnloader(PluginMetadata* pluginMetadata);

signals:
	void removeUISignal(QWidget* widget);

	void loading(const QString& name);
	void loaded(const QString& name, PluginMetadata* pluginMetadata, bool flag);
	void loadError(ErrorList loadError, PluginMetadata* pluginMetadata);

	void unloading(const QString& name);
	void unloaded(const QString& name, PluginMetadata* pluginMetadata);
	void unloadError(UnloadError unloadError, PluginMetadata* pluginMetadata);

	void regPluginMainUI(PluginCalInterface* plugin);
	void backReadAllFile(const QString& dirPath, const QStringList& fileNames);

	void errorReport(ErrorType type, ErrorList error);
	void initCompletePlugin(PluginMetadata* pluginMetadata, PluginCalInterface* plugin);
	//void backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);
	
};
