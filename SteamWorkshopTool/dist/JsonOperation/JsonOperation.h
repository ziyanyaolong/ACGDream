#pragma once

#include <QObject>
#include <QFile>
#include <QString>
#include <QMap>
#include <QStringList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

class JsonOperation : public QObject
{
	Q_OBJECT

private:
	QString json = "";
	QJsonParseError::ParseError error;

public:
	JsonOperation(const QString& str = "", QObject* parent = Q_NULLPTR);
	~JsonOperation();

	const QString& getJson() { return json;  }
	QJsonParseError::ParseError getError() { return error; }
	void setJson(const QString& str) { json = str; }
	QString analyticBackKey(const QString& key, unsigned int pos = 0);
	QString analyticBackValue(const QString& key, unsigned int pos = 0);
	QMap<QString, QString> analytic(const QString& key);
	QMap<QString, QString> analyticAll();
	template <typename ... T>
	QMap<QString, QString> analytics(const QString& key, const T& ... keys);
	QString toString(const QJsonValue& value);

signals:
	void errorAnalytic(QJsonParseError::ParseError);
};
