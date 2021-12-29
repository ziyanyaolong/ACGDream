#include "LoadRes.h"

LoadRes::LoadRes(QObject *parent)
	: QObject(parent)
{
	paths << (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches")
		<< (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/Stars")
		<< (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/Images");
}

LoadRes::~LoadRes()
{
}

QList<LoadRes::Errors> LoadRes::allInit()
{
	QList<LoadRes::Errors> error;

	if (!loadDirs())
	{
		if (!initDirs())
		{
			error << LoadRes::Errors::InitDirError;
		}
	}

	if (!loadStaticWebsitePicture())
	{
		if (!initStaticWebsitePicture())
		{
			error << LoadRes::Errors::InitStaticWebsitePictureError;
		}
	}

	if (!loadText())
	{
		if (!initText())
		{
			error << LoadRes::Errors::InitTextError;
		}
	}

	if (!loadJson())
	{
		if (!initJson())
		{
			error << LoadRes::Errors::InitJsonError;
		}
	}

	return error;
}

bool LoadRes::initDirs()
{
	QDir dir;

	foreach (auto &&i, paths)
	{
		if (!dir.mkdir(i))
		{
			return false;
		}
	}

	return true;
}

bool LoadRes::initStaticWebsitePicture()
{

	return true;
}

bool LoadRes::initText()
{
	return true;
}

bool LoadRes::initJson()
{
	return true;
}

bool LoadRes::loadDirs()
{
	QDir dir;

	foreach(auto && i, paths)
	{
		dir.setPath(i);
		if (!dir.exists())
		{
			return false;
		}
		SteamGet::instance()->addData(("Dirs." + dir.dirName()), dir.path());
	}

	return true;
}

bool LoadRes::loadStaticWebsitePicture()
{
	return true;
}

bool LoadRes::loadText()
{
	return true;
}

bool LoadRes::loadJson()
{
	QFile file(":/SteamWorkshopTool/assets/Config/AnalyticTable.json");

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	JsonOperation json(file.readAll(), this);

	SteamGet::instance()->setParent(this);
	SteamGet::instance()->addData(json.analyticAll());

	return true;
}
