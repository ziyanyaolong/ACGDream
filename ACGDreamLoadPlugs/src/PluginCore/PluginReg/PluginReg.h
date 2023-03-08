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

class PluginReg : public QObject
{
	Q_OBJECT

public:
	enum class ReturnData
	{
		FALSE,
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
		NoMetaData,
		OpenFileError,
		NoAnalytic,
		Unknown
	};

	enum class LoadError
	{
		Normal = 0x0,
		Unknown
	};

	enum class UnloadError
	{
		Normal = 0x0,
		NoLoading,
		NoRegister,
		Unknown
	};

	struct PluginDependencyData
	{
		QPluginLoader* pluginLoader = nullptr;
		QString name = "";																
		quint32 dependencyCount = 0;
		quint32 dependencyNeedCount = 0;
	};

	typedef QList<PluginDependencyData*> PreloadList;
	typedef QMap<QString, PreloadList> PluginMapList;
	typedef QMap<QString, QPluginLoader*> PluginLoaded;
	typedef QMap<QString, QPluginLoader*> PluginMap;

	explicit PluginReg(QObject* parent = Q_NULLPTR);
	virtual ~PluginReg();

private:
	friend class PluginCore;

	PluginMapList pluginPreloadList; //预加载队列
	PluginLoaded pluginLoaded; //已加载队列
	// PluginMap plugins; //所有插件表

	QList<QString> analyticTable;
	QString name;

	void preloaderTest(const QString& name, QPluginLoader* pluginLoader, bool flag);

	PluginCalInterface* wakeUpPreloader(QPluginLoader* pluginLoader);
	PluginCalInterface* pluginInit(QPluginLoader* pluginLoader);

signals:
	void removeUISignal(QWidget* widget);

	void loading(const QString& name);
	void loaded(const QString& name, QPluginLoader* pluginLoader, bool flag);
	void loadError(ErrorList loadError, const QString& name, const QString& tips, QPluginLoader* plugin);

	void unloading(const QString& name);
	void unloaded(const QString& name, QPluginLoader* pluginLoader);
	void unloadError(UnloadError unloadError, const QString& name, QPluginLoader* plugin);

	void regPluginMainUI(PluginCalInterface* plugin);
	void backReadAllFile(const QString& dirPath, const QStringList& fileNames);

	void errorReport(ErrorType type, ErrorList error);
	void initCompletePlugin(QPluginLoader* pluginLoader, PluginCalInterface* plugin);

public slots:
	void readAllFile(const QString& dirPath);
	void loadPlugin(const QString& dirPath, const QString& fileName);
	void loadAllPlugins(const QString& dirPath, const QStringList& fileNames);
	void unloadPlugin(const QString& name, QPluginLoader* pluginLoader = nullptr);
	void unloadAllPlugins();
	//void backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);
	
};
