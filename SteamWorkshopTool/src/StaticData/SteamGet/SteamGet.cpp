#include "SteamGet.h"

SteamGet* SteamGet::_me_ = nullptr;

SteamGet::SteamGet(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<SteamGet::Info>("SteamGet::Info");
	qRegisterMetaType<SteamGet::Event>("SteamGet::Event");
}

SteamGet::~SteamGet()
{
	_me_ = nullptr;
}