#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface(), QObject()
{
}

int SteamWorkshopTool::run(int, char* [], ACGDream const*)
{
	qDebug() << "OK!";
	return 0;
}