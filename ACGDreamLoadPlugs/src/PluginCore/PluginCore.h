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
	void loadAllPlugins(const QString& dirPath, const QStringList& fileNames);
	void unloadPlugin(QPluginLoader* pluginLoader);
	void unloadAllPlugins();
	void backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);

private slots:
	void runPlugin(QPluginLoader* pluginLoader, PluginCalInterface* plugin);
	void setPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);

};
