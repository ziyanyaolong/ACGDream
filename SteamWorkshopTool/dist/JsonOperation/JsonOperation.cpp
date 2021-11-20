#include "JsonOperation.h"

JsonOperation::JsonOperation(const QString& str, QObject *parent)
	: QObject(parent)
{
	json = str;
}

JsonOperation::~JsonOperation()
{
	
}

QString JsonOperation::toString(const QJsonValue& value)
{
    if (value.isBool())
    {
        return QString::number(value.toBool());
    }
    else if (value.isDouble())
    {
        return QString::number(value.toDouble());
    }
    return value.toString();
}

QMap<QString, QString> JsonOperation::analytic(const QString& key)
{
    QMap<QString, QString> list;
    QJsonParseError jsonParserError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toLocal8Bit(), &jsonParserError);
    if (!(!jsonDocument.isNull() && jsonParserError.error == QJsonParseError::NoError))
    {
        error = jsonParserError.error;
        emit this->errorAnalytic(error);
        return list;
    }

    if (jsonDocument.isObject())
    {
        QJsonObject object = jsonDocument.object();
        QString str = key;
        QString keyStr = str;
        int pos = str.indexOf(".");
        if (pos != -1)
        {
            int sizeDot = QString(".").size();
            QJsonValue value;
            while (pos != -1 || str.size() > 0)
            {
                if (pos != -1)
                {
                    keyStr = str.mid(0, pos);
                    str = str.mid(pos + sizeDot, str.size());
                }
                else
                {
                    keyStr = str;
                    str = "";
                    break;
                }
                if (object.contains(keyStr))
                {
                    value = object.value(keyStr);
                    object = value.toObject();
                }
                else
                {
                    keyStr = "";
                    break;
                }
                pos = str.indexOf(".");
            }
        }
        if (object.contains(keyStr))
        {
            QJsonValue value = object.value(keyStr);
            QString strName = toString(value);
            list[key] = strName;
        }
    }

    error = jsonParserError.error;

    if(error != QJsonParseError::NoError)
        emit this->errorAnalytic(error);

    return list;
}

QMap<QString, QString> JsonOperation::analyticAll()
{
    QJsonObject::Iterator it;
    QJsonParseError jsonParserError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toLocal8Bit(), &jsonParserError);
    QJsonObject object = jsonDocument.object();
    QMap<QString, QString> list;
    QMap<QString, QJsonObject> listValue1;
    QMap<QString, QJsonObject> listValue2;

    for (auto it = object.begin(); it != object.end(); it++)
    {
        if (it.value().isObject())
        {
            //listValue1[it.value().toObject()] = it.key() + ".";
            listValue1[QString(it.key() + ".")] = it.value().toObject();
        }
        else
        {
            list[it.key()] = toString(it.value());
        }
        
    }
    while (listValue1.size() > 0)
    {
        listValue2.clear();
        for (auto i = listValue1.begin(); i != listValue1.end(); i++)
        {
            QJsonObject& value = i.value();
            for (auto it = value.begin(); it != value.end(); it++)
            {
                if(it.value().isObject())
                {
                    listValue2[i.key() + it.key() + "."] = it.value().toObject();
                }
                else
                {
                    list[i.key() + it.key()] = toString(it.value());
                }
            }
        }

        listValue1.clear();
        listValue1 = listValue2;
    }

    error = jsonParserError.error;

    if (error != QJsonParseError::NoError)
        emit this->errorAnalytic(error);

    return list;
}

QString JsonOperation::analyticBackKey(const QString& key, unsigned int pos)
{
    QMap<QString, QString> tempList = analytic(key);
    QMap<QString, QString>::iterator i = tempList.begin();
    if (tempList.size() <= 0)
    {
        return "";
    }
    else if (pos < tempList.size())
    {
        for (unsigned int number = 0; number <= pos; number++) i++;
    }

    return i.key();
}

QString JsonOperation::analyticBackValue(const QString& key, unsigned int pos)
{
    QMap<QString, QString> tempList = analytic(key);
    QMap<QString, QString>::iterator i = tempList.begin();
    if (tempList.size() <= 0)
    {
        return "";
    }
    else if(pos < tempList.size() - 1)
    {
        for (unsigned int number = 0; number <= pos; number++) 
            i++;
    }

    return i.value();
}

template <typename ... T>
QMap<QString, QString> JsonOperation::analytics(const QString& key, const T& ... keys)
{
    QMap<QString, QString> list = analytic(key);
    list.insert(analytic(keys...));
    return list;
}