#include "WebsiteAnalytic.h"

WebsiteAnalytic::WebsiteAnalytic(QObject *parent)
	: QObject(parent)
{
	webData = new QString("");
	qRegisterMetaType<ModDataTable>("ModDataTable");
}

WebsiteAnalytic::~WebsiteAnalytic()
{
	clearModList();

	if (webData)
		delete webData;

}

void WebsiteAnalytic::analyticWebsiteData(const QString& data)
{
	*webData = data;
	analyticWebsiteData();
}

QString WebsiteAnalytic::unicodeToUtf8(const QString& str)
{
	QString str2 = str;
	while (1)
	{
		QRegularExpression regExpCode("(?<=\\\\u)[a-z0-9]{4}");
		QRegularExpressionMatch matchCode = regExpCode.match(str2);
		if (matchCode.hasMatch())
		{
			QString tStr = matchCode.captured(0);
			QChar nHex = tStr.toInt(0, 16);
			str2 = str2.replace("\\u" + tStr, nHex);
		}
		else
		{
			break;
		}
	}
	return str2;
}

void WebsiteAnalytic::analyticWebsiteData()
{
	clearModList();
	QString tempStr = webData->mid(
		(webData->indexOf("<div class=\"workshopBrowseItems\">") + QString("<div class=\"workshopBrowseItems\">").size()),
		webData->indexOf("<div class=\"workshopBrowsePaging\">"));
	QString dataTemp("");
	QString jsonTemp("");
	SteamGet* steamGet = SteamGet::instance();
	QString jsonHead1 = steamGet->findData("ItemAnalytic.JsonAnalytic.Head1");
	QString jsonHead2 = steamGet->findData("ItemAnalytic.JsonAnalytic.Head2");
	QString jsonTail1 = steamGet->findData("ItemAnalytic.JsonAnalytic.Tail1");
	QString jsonTail2 = steamGet->findData("ItemAnalytic.JsonAnalytic.Tail2");
	QString itemHead = steamGet->findData("ItemAnalytic.Head");
	QString itemTail = steamGet->findData("ItemAnalytic.Tail");
	QString imageHead1 = steamGet->findData("ItemAnalytic.JsonAnalytic.Image.Head1");
	QString imageHead2 = steamGet->findData("ItemAnalytic.JsonAnalytic.Image.Head2");
	QString imageTail = steamGet->findData("ItemAnalytic.JsonAnalytic.Image.Tail");
	int headPos = tempStr.indexOf(itemHead);
	int tailPos = tempStr.indexOf(itemTail);

	while ((headPos != -1) && (tailPos != -1))
	{
		dataTemp = tempStr.mid(headPos + itemHead.size(), tailPos - (headPos + itemHead.size()));
		ModDataTable* modDataTable = new ModDataTable;
		jsonTemp = dataTemp.mid(dataTemp.indexOf(jsonHead1) + jsonHead1.size(), dataTemp.lastIndexOf(jsonTail1) - (dataTemp.indexOf(jsonHead1) + jsonHead1.size()));
		jsonTemp = jsonTemp.mid(jsonTemp.indexOf(jsonHead2), jsonTemp.lastIndexOf(jsonTail2) + jsonTemp.size() - jsonTemp.indexOf(jsonHead2));
		jsonTemp = jsonTemp.replace("\\r", "");
		JsonOperation json(jsonTemp);
		dataTemp = dataTemp.mid(dataTemp.indexOf(imageHead1) + imageHead1.size());
		dataTemp = dataTemp.mid(dataTemp.indexOf(imageHead2) + imageHead2.size());
		dataTemp = dataTemp.mid(0, dataTemp.indexOf(imageTail));
		modDataTable->image = dataTemp;
		modDataTable->id = json.analyticBackValue("id", 0);
		modDataTable->appid = json.analyticBackValue("appid", 0);
		modDataTable->title = json.analyticBackValue("title", 0);
		finishData.push_back(modDataTable);
		emit this->newModDataTable(*modDataTable);
		tempStr = tempStr.mid(tailPos + itemTail.size(), tempStr.size());
		headPos = tempStr.indexOf(itemHead);
		tailPos = tempStr.indexOf(itemTail);
	}

	emit this->finish();
	emit this->finished(finishData);
}
