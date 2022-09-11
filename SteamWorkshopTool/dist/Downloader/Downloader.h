#pragma once

#include <QObject>

#include "SteamCMD/SteamCMD.h"

class Downloader  : public QObject
{
	Q_OBJECT

private:
	SteamCMD* steamCMD = nullptr;

public:
	Downloader(QObject *parent);
	virtual ~Downloader();

	void init();
};
