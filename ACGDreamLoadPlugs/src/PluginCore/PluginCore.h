#pragma once

#include <QObject>

#include "PluginReg/PluginReg.h"

class PluginCore  : public QObject
{
	Q_OBJECT

private:
	QString pluginDirPath;
	PluginReg* pluginReg;

public:
	explicit PluginCore(QObject *parent = Q_NULLPTR);
	virtual ~PluginCore();

	void pluginPathFiltrate(const QString& dirPath, const QStringList& fileNames);
	void setPluginDirPath(const QString& path) { pluginDirPath = path; }
	const QString& readPluginDirPath() { return pluginDirPath; }

signals:
	void loadPlugin(const QString& dirPath, const QString& fileName);
	void loadAllPlugins(const QString& dirPath);

	void unloadPlugin(const QString& name, PluginMetaData* pluginMetaData = nullptr);
	void unloadAllPlugins();

	void loaded(const QString& name, PluginMetaData* pluginMetaData);
	void unloaded(const QString& name, PluginMetaData* pluginMetaData);

	void removeUISignal(QWidget* widget);
	void regUISignal(PluginCalInterface* plugin, PluginCalInterface::pCreateMainUIPoniterFunc func);

	void backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);

private slots:
	void runPlugin(PluginMetaData* pluginMetaData, PluginCalInterface* plugin);

};
