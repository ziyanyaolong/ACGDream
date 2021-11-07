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
	QVector<PluginCalInterface*> plugins;
	QSharedPointer<QThread> thread;
public:
	PluginReg(QObject *parent = Q_NULLPTR);
	~PluginReg();
	const QVector<PluginCalInterface*>& readPluginList() { return plugins; }

public slots:
	bool loadAllPlugins(const QString & dirPath);
	bool loadPlugin(const QString& filePath);
	void unloadAllPlugins();

};
