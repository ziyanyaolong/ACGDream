#pragma once

#include <QObject>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include <QVector>
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QSharedPointer>

#include "../PluginCalInterface/PluginCalInterface.h"

class PluginReg : public QObject
{
	Q_OBJECT

private:
	friend class ACGDream;
	QMap<QString, QPluginLoader*> plugins;
	QMap<const QPluginLoader*, QWidget*> pluginWidgetList;
	QString name;

public:
	explicit PluginReg(QObject *parent = Q_NULLPTR);
	~PluginReg();
	const QMap<QString, QPluginLoader*>& readPluginList() { return plugins; }

signals:
	void addUISignal(QWidget* gui);
	void removeUISignal(QWidget* gui);
	void loading(QString name);
	void loadError(const QString& name, const QString& info = QString(""));

public slots:
	bool loadAllPlugins(const QString & dirPath);
	bool loadPlugin(const QString& filePath);
	bool unloadPlugin(const QString& plugin);
	void unloadAllPlugins();
	void regMainUIProcess(QWidget* mainWidget);
};
