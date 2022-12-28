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
	steamCMD = nullptr;
}

void Downloader::init()
{
	steamCMD->init();
}