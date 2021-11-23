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
	enum class Way
	{
		Normal = 0x0,
		Overlay = 0x1
	};

	explicit DataBase(QObject *parent = Q_NULLPTR);
	virtual ~DataBase();

	bool findModDataTable(const QString& key);
	bool deleteModDataTable(const QString& key);
	QStringList readModDataTableAll();

public slots:
	bool addModDataTable(const ModDataTable* data, DataBase::Way way = DataBase::Way::Normal);

};
