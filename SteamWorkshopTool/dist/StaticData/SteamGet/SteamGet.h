#pragma once

#include <QObject>
#include <QString>
#include <QMap>

class SteamGet : public QObject
{
	Q_OBJECT

private:
	static SteamGet* _me_;
	QMap<QString, QString> _map;

private:
	enum class Event
	{
		Remove,
		Add,
		Find
	};

	enum class Info
	{
		Normal,
		Error,
		Other
	};

public:
	SteamGet(QObject *parent = Q_NULLPTR);
	~SteamGet();

	static SteamGet* instance() { if (_me_ == nullptr) _me_ = new SteamGet; return _me_; }

signals:
	void removeDataKey(const QString&);
	void removeDataValue(const QString&);

	void addDataKey(const QString&);
	void addDataValue(const QString&);

	void findDataKey(const QString&);
	void findDataValue(const QString&);

	void operationalEvent(SteamGet::Event, SteamGet::Info);

public slots:

	void addData(const QMap<QString, QString>& list)
	{
		for (auto i = list.begin(); i != list.end() ; i++)
		{
			_map[i.key()] = i.value();
		}
	}

	void addData(const QString& key, const QString& value)
	{ 
		_map[key] = value; 
		emit addDataKey(key);
		emit addDataValue(value);
		emit operationalEvent(SteamGet::Event::Add, SteamGet::Info::Normal);
	}

	bool removeData(const QString& key)
	{
		auto i = _map.find(key);
		if (i != _map.end())
		{
			emit removeDataKey(key);
			emit removeDataValue(*i);
			_map.erase(i);
			emit operationalEvent(SteamGet::Event::Remove, SteamGet::Info::Normal);
			return true;
		}
		emit operationalEvent(SteamGet::Event::Remove, SteamGet::Info::Error);
		return false;
	}

	QString getData(const QString& key)
	{
		auto temp = _map.find(key);
		if (temp != _map.end())
		{
			emit findDataKey(key);
			emit findDataValue(temp.value());
			emit operationalEvent(SteamGet::Event::Find, SteamGet::Info::Normal);
			return temp.value();
		}
		emit operationalEvent(SteamGet::Event::Find, SteamGet::Info::Error);
		return "";
	}

};

extern SteamGet steamGet;