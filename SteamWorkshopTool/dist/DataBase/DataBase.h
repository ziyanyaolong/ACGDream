#pragma once

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QCoreApplication>
#include <QStringList>
#include <QMap>

#include "../ModDataTable/ModDataTable.h"

class DataBase : public QObject
{
	Q_OBJECT

public:
	enum class WriteWay
	{
		Normal = 0x0,
		Overlay = 0x1
	};

	enum class Events
	{
		Delete,
		Add,
		All
	};

	struct AddEventDataTable
	{
		DataBase::WriteWay way;
		ModDataTable mod;
	};

private:
	QSqlDatabase database;
	QString sqlLiteName = "SteamWorkshopTool_mod_table";
	QString dataBaseName = QCoreApplication::applicationDirPath() + "/SteamWorkshopTool.db";
	QMap<QString, DataBase::AddEventDataTable> eventAddDataList;
	QStringList eventDeleteDataList;

public:
	explicit DataBase(QObject *parent = Q_NULLPTR);
	virtual ~DataBase();

	bool findModDataTable(const QString& key);
	bool deleteModDataTable(const QString& key);
	QStringList readModDataTableKeyAll();
	ModDataTable readModDataTable(const QString& key);
	QList<ModDataTable> readModDataTableAll();
	bool addModDataTable(const ModDataTable* data, DataBase::WriteWay way = DataBase::WriteWay::Normal);
	
	bool open();
	void close();

public slots:
	void addDataEvent(const ModDataTable* data, DataBase::WriteWay way = DataBase::WriteWay::Normal);
	void deleteDataEvent(const QString& key);
	void removeListEventData(const QString& key, DataBase::Events way);
	void updata(DataBase::Events way = DataBase::Events::All);
};
