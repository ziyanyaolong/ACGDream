#pragma once

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QCoreApplication>

#include "../ModDataTable/ModDataTable.h"

class DataBase : public QObject
{
	Q_OBJECT
private:
	QSqlDatabase database;
	QString sqlLiteName = "SteamWorkshopTool_mod_table";
	QString dataBaseName = QCoreApplication::applicationDirPath() + "/SteamWorkshopTool.db";

public:
	enum Way
	{
		Normal,
		Overlay
	};
	DataBase(QObject *parent);
	~DataBase();

public slots:
	bool addModDataTable(const ModDataTable& data, DataBase::Way way = DataBase::Way::Normal);

};
