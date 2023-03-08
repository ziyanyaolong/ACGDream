#include "WebsiteAnalytic.h"

WebsiteAnalytic::WebsiteAnalytic(QObject *parent)
	: QObject(parent)
{
	webData = new QString("");
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
	QString jsonHead1 = steamGet->getData("ItemAnalytic.JsonAnalytic.Head1");
	QString jsonHead2 = steamGet->getData("ItemAnalytic.JsonAnalytic.Head2");
	QString jsonTail1 = steamGet->getData("ItemAnalytic.JsonAnalytic.Tail1");
	QString jsonTail2 = steamGet->getData("ItemAnalytic.JsonAnalytic.Tail2");
	QString itemHead = steamGet->getData("ItemAnalytic.Head");
	QString itemTail = steamGet->getData("ItemAnalytic.Tail");
	QString imageHead1 = steamGet->getData("ItemAnalytic.JsonAnalytic.Image.Head1");
	QString imageHead2 = steamGet->getData("ItemAnalytic.JsonAnalytic.Image.Head2");
	QString imageTail = steamGet->getData("ItemAnalytic.JsonAnalytic.Image.Tail");
	QString starHead = steamGet->getData("SteamWorkShop.Star.Head");
	QString starTail = steamGet->getData("SteamWorkShop.Star.Tail");
	QString image = "";
	QString star = "";
	int headPos = tempStr.indexOf(itemHead);
	int tailPos = tempStr.indexOf(itemTail);

	while ((headPos != -1) && (tailPos != -1))
	{
		dataTemp = tempStr.mid(headPos + itemHead.size(), tailPos - (headPos + itemHead.size()));
		ModDataTable* modDataTable = new ModDataTable;
		jsonTemp = dataTemp.mid(dataTemp.indexOf(jsonHead1) + jsonHead1.size(), dataTemp.lastIndexOf(jsonTail1) - (dataTemp.indexOf(jsonHead1) + jsonHead1.size()));
		jsonTemp = jsonTemp.mid(jsonTemp.indexOf(jsonHead2), jsonTemp.lastIndexOf(jsonTail2) + jsonTemp.size() - jsonTemp.indexOf(jsonHead2));
		jsonTemp = jsonTemp.replace("\\r", "");
		ACGDREAM_DL::JsonOperation json(jsonTemp);
		image = dataTemp.mid(dataTemp.indexOf(imageHead1) + imageHead1.size());
		image = image.mid(image.indexOf(imageHead2) + imageHead2.size());
		image = image.mid(0, image.indexOf(imageTail));
		star = dataTemp.mid(dataTemp.indexOf(starHead) + starHead.size(), dataTemp.lastIndexOf(starTail) - (dataTemp.indexOf(starHead) + starHead.size()));
		modDataTable->image = image;
		modDataTable->star = star;
		modDataTable->id = json.analyticBackValue("id", 0).toString();
		modDataTable->appid = json.analyticBackValue("appid", 0).toString();
		modDataTable->title = json.analyticBackValue("title", 0).toString();
		finishData.push_back(modDataTable);
		emit this->newModDataTable(*modDataTable);
		tempStr = tempStr.mid(tailPos + itemTail.size(), tempStr.size());
		headPos = tempStr.indexOf(itemHead);
		tailPos = tempStr.indexOf(itemTail);
	}

	emit this->finish();
	emit this->finished(finishData);
}
