#include "SteamCMD.h"

SteamCMD::SteamCMD(QObject *parent)
	: QObject(parent)
{}

SteamCMD::~SteamCMD()
{
	this->exit();
}

bool SteamCMD::init()
{
	////_popen("wmic", "r");
	//FILE* fp = _popen("wmic cpu get Name", "r");
	//if (!fp)
	//{
	//	return false;
	//}

	//QFile file;
	//file.open(fp, QIODevice::ReadOnly);
	//QString str = file.readAll();
	//file.close();

	//_pclose(fp);

	//qDebug() << str;

	//system("steamcmd && login anonymous && exit && pause");
	return true;
}

void SteamCMD::exit()
{

}