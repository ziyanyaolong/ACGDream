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
#include <QVariantMap>

namespace ACGDREAM_DL
{
	class JsonOperation : public QObject
	{
		Q_OBJECT

	private:
		QString json = "";
		QJsonObject* jsonObject = nullptr;
		QJsonParseError::ParseError error;

		bool JsonDocumentSet(QJsonDocument& jsonDocument, QJsonParseError& jsonParserError);

	public:
		JsonOperation(const QString& str = "", QObject* parent = Q_NULLPTR);
		virtual ~JsonOperation();

		inline const QString& getJson() { return json; }
		inline QJsonParseError::ParseError getError() { return error; }
		inline void setJson(const QString& str) { json = str; }
		inline void setJsonObject(QJsonObject* object) { jsonObject = object; }

		QVariant analyticBackKey(const QString& key, unsigned int pos = 0);
		QVariant analyticBackValue(const QString& key, unsigned int pos = 0);
		QVariantMap analytic(const QString& key);
		QVariantMap analyticAll();

		template <typename ... T>
		QVariantMap analytics(const QString& key, const T& ... keys);

	signals:
		void errorAnalytic(QJsonParseError::ParseError);
	};
}