#include "LoadRes.h"

LoadRes::LoadRes(QObject *parent)
	: QObject(parent)
{
	dirPaths << (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches")
		<< (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/Stars")
		<< (QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/Images");

	jsonPaths << QString(":/SteamWorkshopTool/assets/Config/AnalyticTable.json");

	QFile file(QCoreApplication::applicationDirPath() + "/Config/SteamWorkshopTool/AnalyticTable.json");
	QFileInfo fileInfo(file);

	if (fileInfo.isFile())
	{
		return;
	}

	file.setFileName(jsonPaths[0]);

	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}

	QByteArray line = file.readAll();

	file.close();

	file.setFileName(QCoreApplication::applicationDirPath() + "/Config/SteamWorkshopTool/AnalyticTable.json");

	if (!file.open(QIODevice::WriteOnly))
	{
		return;
	}

	file.write(line);

	file.close();
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

	if (loadJson())
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

	foreach (auto &&i, dirPaths)
	{
		if (dir.exists())
		{
			continue;
		}
		else if (!dir.mkdir(i))
		{
			continue;
		}
		SteamGet::instance()->addData(("Dirs." + dir.dirName()), i);
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
	QFile file;

	foreach (auto &&i, jsonPaths)
	{
		if (file.exists())
			continue;

		file.setFileName(i);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			continue;
		}

		ACGDREAM_DL::JsonOperation json(file.readAll(), this);

		SteamGet::instance()->addData(json.analyticAll());
	}
	return true;
}

bool LoadRes::loadDirs()
{
	QDir dir;

	foreach(auto && i, dirPaths)
	{
		dir.setPath(i);
		if (!dir.exists())
		{
			return false;
		}
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
	QFile file;

	foreach(auto && i, jsonPaths)
	{
		file.setFileName(i);
		if (!file.exists())
			return false;
	}

	return true;
}
