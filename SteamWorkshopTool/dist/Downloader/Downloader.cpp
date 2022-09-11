#include "Downloader.h"

Downloader::Downloader(QObject *parent)
	: QObject(parent)
{
	steamCMD = new SteamCMD(this);
}

Downloader::~Downloader()
{
	if (steamCMD)
	{
		steamCMD->deleteLater();
	}
}

void Downloader::init()
{
	steamCMD->init();
}