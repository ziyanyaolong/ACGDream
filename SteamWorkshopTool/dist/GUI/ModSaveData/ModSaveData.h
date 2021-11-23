#pragma once

#include <QObject>

#include "../../ModDataTable/ModDataTable.h"
#include "../../Network/WebCrawler/WebCrawler.h"
#include "../Components/ListWidgetItemWidget/ListWidgetItemWidget.h"

struct ModSaveData
{
	QVector<ListWidgetItemWidget*> list;
	QList<WebCrawler*> webList;
	QStringList datalist;
};