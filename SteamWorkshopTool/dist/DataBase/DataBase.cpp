#include "DataBase.h"

DataBase::DataBase(QObject *parent)
	: QObject(parent)
{
    if (QSqlDatabase::contains(sqlLiteName))
    {
        database = QSqlDatabase::database(sqlLiteName);
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE", sqlLiteName);
        database.setDatabaseName(dataBaseName);
    }
    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
        return;
    }
    database.close();
}

DataBase::~DataBase()
{
    database.close();
}

bool DataBase::addModDataTable(const ModDataTable* data)
{
    if (!database.open())
        return false;
    QSqlQuery query(database);
    query.exec("create table student(mkey text primary key, appid int, id int, title text, image text)");
    QString insertSql = "insert into student(mkey,appid,id,title,image) values('%1',%2,%3,'%4','%5')";
    insertSql = insertSql.arg(data->appid + data->id, data->appid, data->id, data->title, "");
    query.exec(insertSql);
    database.close();
    return true;
}