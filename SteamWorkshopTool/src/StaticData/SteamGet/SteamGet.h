#pragma once

#include <QObject>
#include <QString>
#include <QMap>

#include <QVariant>

class SteamGet : public QObject
{
	Q_OBJECT

private:
	static SteamGet* _me_;
	QVariantMap _map;

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
	virtual ~SteamGet();

	static SteamGet* instance() { if (_me_ == nullptr) _me_ = new SteamGet; return _me_; }
	static void deleteSteamGet() { if (_me_) _me_->deleteLater(); _me_ = nullptr; }

signals:
	void removeDataKey(const QString&);
	void removeDataValue(const QString&);

	void addDataKey(const QString&);
	void addDataValue(const QString&);

	void findDataKey(const QString&);
	void findDataValue(const QString&);

	void operationalEvent(SteamGet::Event, SteamGet::Info);

public slots:

	void addData(const QVariantMap& list)
	{
		_map = list;
	}

	void addData(const QString& key, const QVariant& value)
	{
		auto it = _map.find(key);
		if (it == _map.end())
		{
			it.value() = value;
		}
		else
		{
			_map.insert(key, value);
		}

		emit addDataKey(key);
		emit addDataValue(value.toString());
		emit operationalEvent(SteamGet::Event::Add, SteamGet::Info::Normal);
	}

	bool removeData(const QString& key)
	{
		auto i = _map.find(key);
		if (i != _map.end())
		{
			emit removeDataKey(key);
			emit removeDataValue(i.value().toString());
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
			emit findDataValue(temp.value().toString());
			emit operationalEvent(SteamGet::Event::Find, SteamGet::Info::Normal);
			return temp.value().toString();
		}
		emit operationalEvent(SteamGet::Event::Find, SteamGet::Info::Error);
		return "";
	}

};

extern SteamGet steamGet;