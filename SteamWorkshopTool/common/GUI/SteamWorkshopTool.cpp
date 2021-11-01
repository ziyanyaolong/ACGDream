#include "SteamWorkshopTool.h"

SteamWorkshopTool::SteamWorkshopTool()
	:	PluginCalInterface()
{
	ui.setupUi(this);
	qDebug() << sizeof(void*) << "OK!";
}

SteamWorkshopTool::~SteamWorkshopTool()
{
}

void SteamWorkshopTool::run()
{
	setOpenEventLoop(true);
}