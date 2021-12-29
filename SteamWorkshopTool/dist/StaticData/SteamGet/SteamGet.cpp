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
	if (_me_)
	{
		_me_->deleteLater();
		_me_ = nullptr;
	}
}