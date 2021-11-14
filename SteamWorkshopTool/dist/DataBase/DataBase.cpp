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

bool DataBase::addModDataTable(const ModDataTable& data, DataBase::Way way)
{
    if (!database.open())
        return false;
    QSqlQuery query(database);
    query.exec("create table student(mkey text primary key, appid int, id int, title text, image text, isSubscribe int)");
    QString insertSql;
    if (way == DataBase::Way::Normal)
    {
        insertSql = "insert or ignore into student(mkey,appid,id,title,image,isSubscribe) values('%1',%2,%3,'%4','%5',%6)";
    }
    else if(way == DataBase::Way::Overlay)
    {
        insertSql = "insert or replace into student(mkey,appid,id,title,image,isSubscribe) values('%1',%2,%3,'%4','%5',%6)";
    }
    else
    {
        qDebug() << "error way!";
    }
    
    insertSql = insertSql.arg(data.appid + data.id, data.appid, data.id, data.title, data.image, QString::number(static_cast<int>(data.isSubscribe)));
    query.exec(insertSql);
    database.close();
    return true;
}