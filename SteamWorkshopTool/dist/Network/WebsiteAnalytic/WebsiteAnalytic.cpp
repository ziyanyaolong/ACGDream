#include "WebsiteAnalytic.h"

WebsiteAnalytic::WebsiteAnalytic(QObject *parent)
	: QObject(parent)
{
	webData = new QString("");
	qRegisterMetaType<ModDataTable>("ModDataTable");
	matchList.push_back("id");
	matchList.push_back("appid");
	matchList.push_back("title");
}

WebsiteAnalytic::~WebsiteAnalytic()
{
	if (webData)
		delete webData;

	qDeleteAll(finishData.begin(), finishData.end());
	finishData.clear();
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
	finishData.clear();
	QString tempStr = webData->mid(
		(webData->indexOf("<div class=\"workshopBrowseItems\">") + QString("<div class=\"workshopBrowseItems\">").size()),
		webData->indexOf("<div class=\"workshopBrowsePaging\">"));
	QString dataTemp("");
	int headPos = tempStr.indexOf(head);
	int tailPos = tempStr.indexOf(tail);
	while ((headPos != -1) && (tailPos != -1))
	{
		dataTemp = tempStr.mid(headPos, tailPos + tail.size());
		ModDataTable* modDataTable = new ModDataTable;
		QString js = dataTemp.mid(dataTemp.indexOf(jsHead) + jsHead.size(), dataTemp.indexOf(jsTail));
		dataTemp = dataTemp.mid(dataTemp.indexOf(imageHead) + imageHead.size());
		dataTemp = dataTemp.mid(0, dataTemp.indexOf(imageTail));
		modDataTable->image = dataTemp;
		int number = 0;
		QRegularExpression regExp0("(?<=\"id\":\").*?(?=\")");
		QRegularExpressionMatch match0 = regExp0.match(js);
		QRegularExpression regExp1("(?<=\"appid\":).*?(?=\\})");
		QRegularExpressionMatch match1 = regExp1.match(js);
		QRegularExpression regExp2("(?<=\"title\":\").*?(?=\")");
		QRegularExpressionMatch match2 = regExp2.match(js);
		if (match0.hasMatch())
		{
			modDataTable->id = match0.captured(0);
		}
		if (match1.hasMatch())
		{
			modDataTable->appid = match1.captured(0);
		}
		if (match2.hasMatch())
		{
			modDataTable->title = unicodeToUtf8(match2.captured(0));
		}
		finishData.push_back(modDataTable);
		emit this->newModDataTable(*modDataTable);
		tempStr = tempStr.mid(tailPos + tail.size(), tempStr.size());
		headPos = tempStr.indexOf(head);
		tailPos = tempStr.indexOf(tail);
	}
	
	emit this->finish(finishData);
}