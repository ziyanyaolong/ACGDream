#include "SteamGet.h"

static SteamGet steamGet;
SteamGet* SteamGet::_me_ = &steamGet;

SteamGet::SteamGet(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<SteamGet::Info>("SteamGet::Info");
	qRegisterMetaType<SteamGet::Event>("SteamGet::Event");
}

SteamGet::~SteamGet()
{
}