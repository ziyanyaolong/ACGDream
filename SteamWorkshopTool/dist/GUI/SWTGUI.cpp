#include "SWTGUI.h"

SWTGUI::SWTGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	connect(ui.pushButton, &QPushButton::clicked, this, &SWTGUI::pushButtonResponse_WebAddress);
}

SWTGUI::~SWTGUI()
{
	ui.listWidget->clear();
	qDeleteAll(list);
	list.clear();
}

void SWTGUI::pushButtonResponse_WebAddress()
{
	emit this->haveWebAddress(QString("https://steamcommunity.com/workshop/browse/?appid=" + ui.lineEdit->text()));
}

void SWTGUI::addMod(const ModDataTable& mod)
{
	QListWidgetItem* listWidgetItem = new QListWidgetItem;
	ListWidgetItemWidget* listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget, listWidgetItem);
	list.push_back(listWidgetItemWidget);
	listWidgetItem->setSizeHint(QSize(512, 128));
	ui.listWidget->addItem(listWidgetItem);
	ui.listWidget->setItemWidget(listWidgetItem, listWidgetItemWidget);
	listWidgetItemWidget->setTitle(mod.title);
	if (mod.isSubscribe)
	{
		listWidgetItemWidget->setSubscription(true);
	}
	if (mod.image != "")
	{
		QPixmap pixmap0;
		if (pixmap0.load(QString(QCoreApplication::applicationDirPath() + "/Caches/" + mod.appid + "_" + mod.id + ".png")))
		{
			listWidgetItemWidget->setImage(pixmap0);
		}
		else
		{
			QDir dir(QCoreApplication::applicationDirPath() + "/Caches");
			if (!dir.exists())
				dir.mkdir(QCoreApplication::applicationDirPath() + "/Caches");
			WebCrawler* webCrawler = new WebCrawler(this);
			webCrawler->otherData.push_back(new ModDataTable(mod));
			webCrawler->otherData.push_back(listWidgetItemWidget);
			webCrawler->websiteLink(mod.image);
			connect(webCrawler, &WebCrawler::finish, this, [&](const QByteArray& data) {
				const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
				ListWidgetItemWidget* tListWidgetItemWidget = static_cast<ListWidgetItemWidget*>((static_cast<WebCrawler*>(sender()))->otherData[1]);
				if ((tMod == nullptr) || (tListWidgetItemWidget == nullptr))
					return;
				QPixmap temp;
				if (!temp.loadFromData(data))
					qDebug() << "error loadFromData!";
				if (!temp.save(QCoreApplication::applicationDirPath() + "/Caches/" + tMod->appid + "_" + tMod->id + ".png"))
					qDebug() << "error save Pixmap!" << QCoreApplication::applicationDirPath() + QString("/Caches/" + tMod->appid + "_" + tMod->id + ".png");
				delete tMod;
				tListWidgetItemWidget->setImage(temp);
				((WebCrawler*)sender())->deleteLater();
				});
		}
	}
}