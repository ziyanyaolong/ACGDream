#include "WebsiteAnalytic.h"

WebsiteAnalytic::WebsiteAnalytic(QObject *parent)
	: QObject(parent)
{
	webData = new QString("");
	matchList.push_back("id");
	matchList.push_back("appid");
	matchList.push_back("title");
}

WebsiteAnalytic::~WebsiteAnalytic()
{
	if (webData)
		delete webData;
}

void WebsiteAnalytic::analyticWebsiteData(const QString& data)
{
	*webData = data;
	analyticWebsiteData();
}

void WebsiteAnalytic::analyticWebsiteData()
{
	finishData.clear();
	QString tempStr = webData->mid(
		(webData->indexOf("<div class=\"workshopBrowseItems\">") + QString("<div class=\"workshopBrowseItems\">").size()),
		webData->indexOf("<div class=\"workshopBrowsePaging\">"));
	QString dataTemp("");
	//"\"<img class=\"workshopItemPreviewImage  aspectratio_16x9\" src=\"";
	int headPos = tempStr.indexOf(head);
	int tailPos = tempStr.indexOf(tail);
	while ((headPos != -1) && (tailPos != -1))
	{
		dataTemp = tempStr.mid(headPos, tailPos + tail.size());
		WebDataTable webDataTable;
		QString js = dataTemp.mid(dataTemp.indexOf(jsHead) + jsHead.size(), dataTemp.indexOf(jsTail));
		foreach (QString match, matchList)
		{
			QRegExp regExp("(?<=(\"id\":\"))");
			//regExp.setMinimal(true);
			for (int pos = 0; pos = regExp.indexIn(js, pos), pos >= 0; pos += regExp.matchedLength())
			{
				qDebug() << pos; //返回查找到序号
				qDebug() << regExp.cap(0);//返回匹配到的字串
				webDataTable.id = regExp.cap(0).toInt();
			}
		}
		finishData.push_back(webDataTable);
		tempStr = tempStr.mid(tailPos + tail.size(), tempStr.size());
		headPos = tempStr.indexOf(head);
		tailPos = tempStr.indexOf(tail);
	}
	
	emit this->finish(finishData);
}