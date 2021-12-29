#pragma once

#include <QObject>
#include <QList>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

#include "../StaticData/StaticData.h"
#include "../JsonOperation/JsonOperation.h"
#include "../Network/WebCrawler/WebCrawler.h"

class LoadRes : public QObject
{
	Q_OBJECT

private:
	QStringList paths;

public:
	enum class Errors
	{
		InitDirError,
		InitStaticWebsitePictureError,
		InitTextError,
		InitJsonError
	};

	LoadRes(QObject *parent = Q_NULLPTR);
	virtual ~LoadRes();

	QList<LoadRes::Errors> allInit();
	bool initDirs();
	bool initStaticWebsitePicture();
	bool initText();
	bool initJson();

	bool loadDirs();
	bool loadStaticWebsitePicture();
	bool loadText();
	bool loadJson();
};
