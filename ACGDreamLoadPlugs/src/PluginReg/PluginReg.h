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
#include <QException>

#include "../PluginCalInterface/PluginCalInterface.h"

class PluginReg : public QObject
{
	Q_OBJECT

private:
	friend class ACGDream;
	QMap<QString, QPluginLoader*> plugins;
	QString name;

public:
	explicit PluginReg(QObject* parent = Q_NULLPTR);
	virtual ~PluginReg();
	const QMap<QString, QPluginLoader*>& readPluginList() { return plugins; }

signals:
	void removeUISignal(QWidget* widget);
	void loading(QString name);
	void loadError(const QString& name, const QString& info = QString(""));
	void regPluginMainUI(PluginCalInterface* plugin);

protected slots :
	bool loadPlugin(const QString& filePath);
	bool loadAllPlugins(const QString& dirPath);
	bool unloadPlugin(const QString& plugin);
	void unloadAllPlugins();
	void backPluginMainUI(PluginCalInterface* plugin, QWidget* mainWidget);
};
