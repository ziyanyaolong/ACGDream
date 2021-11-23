#include "DataBase.h"

DataBase::DataBase(QObject *parent)
	: QObject(parent)
{
    qRegisterMetaType<DataBase::Way>("DataBase::Way");
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

//temp.appid = query.value(1).toString();
//temp.id = query.value(2).toString();
//temp.title = query.value(3).toString();
//temp.image = query.value(4).toString();

QStringList DataBase::readModDataTableAll()
{
    QStringList list;
    if (!database.open())
        return list;
    QSqlQuery query(database);
    query.exec(QString("select * from student"));
    while (query.next())
    {
        list.push_back(query.value(0).toString());
    }
    database.close();
    return list;
}

bool DataBase::findModDataTable(const QString& key)
{
    if (!database.open())
        return false;
    QSqlQuery query(database);
    query.exec(QString("select * from student where mkey = " + key));
    if (query.next())
    {
        database.close();
        return true;
    }
    database.close();
    return false;
}

bool DataBase::deleteModDataTable(const QString& key)
{
    if (!database.open())
        return false;
    QSqlQuery query(database);
    query.exec(QString("delete from student where mkey = " + key));
    database.close();
    return true;
}

bool DataBase::addModDataTable(const ModDataTable* data, DataBase::Way way)
{
    if (data && !database.open())
        return false;
    QSqlQuery query(database);
    query.exec(QString("create table student(mkey text primary key, appid int, id int, title text, image text, updateTime text)"));
    QString insertSql;
    if (way == DataBase::Way::Normal)
    {
        insertSql = "insert or ignore into student(mkey,appid,id,title,image,updateTime) values('%1',%2,%3,'%4','%5','%6')";
    }
    else if(way == DataBase::Way::Overlay)
    {
        insertSql = "insert or replace into student(mkey,appid,id,title,image,updateTime) values('%1',%2,%3,'%4','%5','%6')";
    }
    else
    {
        qDebug() << "error way!";
    }
    
    insertSql = insertSql.arg(data->appid + data->id, data->appid, data->id, data->title, data->image, "null");
    query.exec(insertSql);
    database.close();
    return true;
}