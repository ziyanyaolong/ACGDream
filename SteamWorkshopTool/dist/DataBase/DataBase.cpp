#include "DataBase.h"

DataBase::DataBase(QObject *parent)
	: QObject(parent)
{
	qRegisterMetaType<DataBase::WriteWay>("DataBase::WriteWay");
	qRegisterMetaType<DataBase::Events>("DataBase::Events");
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

bool DataBase::open()
{
	if (!database.open())
		return false;

	return true;
}

void DataBase::close()
{
	database.close();
}

QStringList DataBase::readModDataTableKeyAll()
{
	QStringList list;

	if (!database.isOpen())
		return list;

	QSqlQuery query(database);

	query.exec(QString("select * from student"));

	while (query.next())
	{
		list.push_back(query.value(0).toString());
	}

	return list;
}

ModDataTable DataBase::readModDataTable(const QString& key)
{

	ModDataTable mod;

	if (!database.isOpen())
		return mod;

	QSqlQuery query(database);

	query.exec(QString("select * from student where mkey = " + key));

	if (query.next())
	{
		mod.appid = query.value(1).toString();
		mod.id = query.value(2).toString();
		mod.title = query.value(3).toString();
		mod.image = query.value(4).toString();
		mod.isSubscribe = true;
	}

	return mod;
}

QList<ModDataTable> DataBase::readModDataTableAll()
{
	QList<ModDataTable> list;
	
	if (!database.isOpen())
		return list;

	QSqlQuery query(database);

	query.exec(QString("select * from student"));

	while (query.next())
	{
		ModDataTable mod;
		mod.appid = query.value(1).toString();
		mod.id = query.value(2).toString();
		mod.title = query.value(3).toString();
		mod.image = query.value(4).toString();
		mod.isSubscribe = true;
		list.push_back(mod);
	}

	return list;
}

bool DataBase::findModDataTable(const QString& key)
{
	if (!database.isOpen())
		return false;

	QSqlQuery query(database);

	query.exec(QString("select * from student where mkey = " + key));

	if (query.next())
	{
		return true;
	}

	return false;
}

bool DataBase::deleteModDataTable(const QString& key)
{
	if (!database.isOpen())
		return false;

	QSqlQuery query(database);

	query.exec(QString("delete from student where mkey = " + key));

	return true;
}

bool DataBase::addModDataTable(const ModDataTable* data, DataBase::WriteWay way)
{
	if (data && !database.isOpen())
		return false;

	QSqlQuery query(database);

	query.exec(QString("create table student(mkey text primary key, appid int, id int, title text, image text, star text, updateTime text)"));
	
	QString insertSql;

	if (way == DataBase::WriteWay::Normal)
	{
		insertSql = "insert or ignore into student(mkey,appid,id,title,image,star,updateTime) values('%1',%2,%3,'%4','%5','%6','%7')";
	}
	else if(way == DataBase::WriteWay::Overlay)
	{
		insertSql = "insert or replace into student(mkey,appid,id,title,image,star,updateTime) values('%1',%2,%3,'%4','%5','%6','%7')";
	}
	else
	{
		qDebug() << "error way!";
	}
	
	insertSql = insertSql.arg(data->appid + data->id, data->appid, data->id, data->title, data->image, data->star, "null");
	query.exec(insertSql);

	return true;
}

void DataBase::deleteDataEvent(const QString& key)
{
	if (eventDeleteDataList.count(key) > 0)
		return;

	eventDeleteDataList.join(key);
	eventDeleteDataList.append(key);
}

void DataBase::removeListEventData(const QString& key, DataBase::Events way)
{
	switch (way)
	{
	case DataBase::Events::Delete:
		eventDeleteDataList.removeOne(key);
		break;
	case DataBase::Events::Add:
		eventAddDataList.remove(key);
		break;

	case DataBase::Events::All:
		eventDeleteDataList.removeOne(key);
		eventAddDataList.remove(key);
		break;

	default:
		break;
	}
}

void DataBase::addDataEvent(const ModDataTable* data, DataBase::WriteWay way)
{
	if (data == nullptr)
		return;

	QString str = data->appid + data->id;

	if (eventAddDataList.count(str) > 0)
		return;

	AddEventDataTable table;

	table.way = way;
	table.mod = *data;

	eventAddDataList[str] = table;
}

void DataBase::updata(DataBase::Events way)
{
	if (!database.isOpen())
		return;

	database.transaction();

	QSqlQuery query(database);

	switch (way)
	{
	case DataBase::Events::Delete:
		foreach (auto i, eventDeleteDataList)
		{
			this->deleteModDataTable(i);
		}
		eventDeleteDataList.clear();
		break;
	case DataBase::Events::Add:
		for (auto i = eventAddDataList.begin(); i != eventAddDataList.end(); i++)
		{
			this->addModDataTable(&(i.value().mod), i.value().way);
		}
		eventAddDataList.clear();
		break;
	case DataBase::Events::All:
		foreach(auto i, eventDeleteDataList)
		{
			this->deleteModDataTable(i);
		}
		eventDeleteDataList.clear();

		for (auto i = eventAddDataList.begin(); i != eventAddDataList.end(); i++)
		{
			this->addModDataTable(&(i.value().mod), i.value().way);
		}
		eventAddDataList.clear();
		break;
	default:
		break;
	}
	
	database.commit();
}
