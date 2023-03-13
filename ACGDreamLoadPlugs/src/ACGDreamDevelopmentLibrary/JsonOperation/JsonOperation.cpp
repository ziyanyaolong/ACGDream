#include "JsonOperation.h"

ACGDREAM_DL::JsonOperation::JsonOperation(const QString& str, QObject *parent)
	: QObject(parent)
{
	json = str;
}

ACGDREAM_DL::JsonOperation::~JsonOperation()
{
	
}

bool ACGDREAM_DL::JsonOperation::JsonDocumentSet(QJsonDocument& jsonDocument, QJsonParseError& jsonParserError)
{
    if (!this->json.isEmpty())
    {
        jsonDocument = QJsonDocument::fromJson(json.toLocal8Bit(), &jsonParserError);
    }
    else if (this->jsonObject != nullptr)
    {
        jsonDocument.setObject(*(this->jsonObject));
    }
    else
    {
        return false;
    }

    return true;
}

QVariantMap ACGDREAM_DL::JsonOperation::analytic(const QString& key)
{
    QVariantMap list;
    QJsonParseError jsonParserError;
    QJsonDocument jsonDocument;
    
    this->JsonDocumentSet(jsonDocument, jsonParserError);
    
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
            QString strName = value.toString();
            list[key] = strName;
        }
    }

    error = jsonParserError.error;

    if(error != QJsonParseError::NoError)
        emit this->errorAnalytic(error);

	return list;
}

QVariantMap ACGDREAM_DL::JsonOperation::analyticAll()
{
    QJsonObject::Iterator it;
    QJsonParseError jsonParserError;
    QJsonDocument jsonDocument;

	this->JsonDocumentSet(jsonDocument, jsonParserError);

    QJsonObject object = jsonDocument.object();
    QVariantMap list;
    QVariantMap listValue1;
    QVariantMap listValue2;

    for (auto it = object.begin(); it != object.end(); it++)
    {
        if (it.value().isObject())
        {
            listValue1[QString(it.key() + ".")] = it.value().toObject();
		}
        else if (it.value().isArray())
        {
            list[it.key()] = it.value().toArray().toVariantList();
        }
        else
        {
            list[it.key()] = it.value().toString();
        }
        
    }

    while (!(listValue1.isEmpty()))
    {
        listValue2.clear();
        for (auto i = listValue1.begin(); i != listValue1.end(); i++)
        {
            QJsonObject value = i.value().toJsonObject(); 
            for (auto it = value.begin(); it != value.end(); it++)
            {
                if(it.value().isObject())
                {
                    listValue2[i.key() + it.key() + "."] = it.value().toObject();
                }
				else if (it.value().isArray())
				{
                    list[i.key() + it.key()] = it.value().toArray().toVariantList();
				}
                else
                {
                    list[i.key() + it.key()] = it.value().toString();
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

QVariant ACGDREAM_DL::JsonOperation::analyticBackKey(const QString& key, unsigned int pos)
{
    QVariantMap tempList = analytic(key);
    QVariantMap::iterator i = tempList.begin();
    if (tempList.size() <= 0)
    {
        return QVariant();
    }
    else if (pos < tempList.size())
    {
        for (unsigned int number = 0; number <= pos; number++) i++;
    }

    return i.key();
}

QVariant ACGDREAM_DL::JsonOperation::analyticBackValue(const QString& key, unsigned int pos)
{
    QVariantMap tempList = analytic(key);
    QVariantMap::iterator i = tempList.begin();
    if (tempList.size() <= 0)
    {
        return QVariant();
    }
    else if(pos < tempList.size() - 1)
    {
        for (unsigned int number = 0; number <= pos; number++) 
            i++;
    }

    return i.value();
}

template <typename ... T>
QVariantMap ACGDREAM_DL::JsonOperation::analytics(const QString& key, const T& ... keys)
{
    QVariantMap jsonMap = analytic(key);
    jsonMap.insert(analytic(keys...));
    return jsonMap;
}