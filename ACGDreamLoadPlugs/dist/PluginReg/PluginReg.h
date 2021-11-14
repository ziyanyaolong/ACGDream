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
	QVector<PluginCalInterface*> plugins;
	QSharedPointer<QThread> thread;

public:
	explicit PluginReg(QObject *parent = Q_NULLPTR);
	~PluginReg();
	const QVector<PluginCalInterface*>& readPluginList() { return plugins; }

signals:
	void addGuiSignal(QWidget* gui);

public slots:
	bool loadAllPlugins(const QString & dirPath);
	bool loadPlugin(const QString& filePath);
	void unloadAllPlugins();
};
