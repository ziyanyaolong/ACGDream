#pragma once

#include <QObject>
#include <QCoreApplication>
#include <QFile>
#include <QDebug>

class SteamCMD  : public QObject
{
	Q_OBJECT

public:
	SteamCMD(QObject *parent);
	virtual ~SteamCMD();

	bool init();
	void exit();
};
