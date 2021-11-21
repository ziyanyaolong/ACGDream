#include "SWTGUI.h"

SWTGUI::SWTGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	connect(ui.pushButton, &QPushButton::clicked, this, [&]() {
		clearModList();
		datalist.clear();
		datalist.push_back(ui.lineEdit->text());
		datalist.push_back(ui.lineEdit_Search->text());
		datalist.push_back(ui.label_page->text());
		emit this->webAddress(datalist);
		});

	connect(ui.pushButton_Previous, &QPushButton::clicked, this, [&]() {
		ui.label_page->setText(QString::number(ui.label_page->text().toULongLong() - 1));
		emit ui.pushButton->clicked();
		});

	connect(ui.pushButton_Next, &QPushButton::clicked, this, [&]() {
		ui.label_page->setText(QString::number(ui.label_page->text().toULongLong() + 1));
		emit ui.pushButton->clicked();
		});

	connect(ui.pushButton_Search, &QPushButton::clicked, this, [&]() {
		ui.label_page->setText(QString::number(1));
		emit ui.pushButton->clicked();
		});

	connect(ui.commandLinkButton_Export, &QCommandLinkButton::clicked, this, [&]() {
		emit this->exportData(QFileDialog::getExistingDirectory(this));
		});

	connect(ui.commandLinkButton_Import, &QCommandLinkButton::clicked, this, [&]() {
		emit this->importData(QFileDialog::getOpenFileName(this));
		});

	connect(ui.commandLinkButton_ClearCache, &QCommandLinkButton::clicked, this, &SWTGUI::clearCache);

	connect(ui.commandLinkButton_Update, &QCommandLinkButton::clicked, this, &SWTGUI::updateMod);
}

SWTGUI::~SWTGUI()
{
	clearModList();
}

void SWTGUI::clearModList()
{
	foreach(auto i, webList)
	{
		disconnect(i, 0 ,0 ,0);
		i->deleteLater();
	}
	foreach (auto i, list)
	{
		disconnect(i);
		i->close();
		i->deleteLater();
	}
	list.clear();
	webList.clear();
	ui.listWidget->clear();
}

void SWTGUI::addMod(const ModDataTable& mod)
{
	QListWidgetItem* listWidgetItem = new QListWidgetItem;
	ListWidgetItemWidget* listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget, listWidgetItem);
	connect(listWidgetItemWidget, &ListWidgetItemWidget::pushButtonResponse_Subscription, this, [&](bool isSubscription) {
		ListWidgetItemWidget* temp = static_cast<ListWidgetItemWidget*>(sender());
		emit this->subscription(isSubscription, temp->readId());
		}); 
	list.push_back(listWidgetItemWidget);
	listWidgetItem->setSizeHint(QSize(listWidgetItemWidget->width(), listWidgetItemWidget->height()));
	ui.listWidget->addItem(listWidgetItem);
	ui.listWidget->setItemWidget(listWidgetItem, listWidgetItemWidget);
	listWidgetItemWidget->setTitle(mod.title);
	listWidgetItemWidget->setId(QString(mod.appid + mod.id));
	listWidgetItemWidget->setSubscription(mod.isSubscribe);
	if (mod.isSubscribe)
	{
		listWidgetItemWidget->setSubscription(true);
	}
	if (mod.image != "")
	{
		QPixmap pixmap0;
		if (pixmap0.load(QString(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/" + mod.appid + "_" + mod.id + ".png")))
		{
			listWidgetItemWidget->setImage(pixmap0);
		}
		else
		{
			QDir dir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
			if (!dir.exists())
				dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
			WebCrawler* webCrawler = new WebCrawler(this);
			webList.push_back(webCrawler);
			webCrawler->otherData.push_back(new ModDataTable(mod));
			webCrawler->otherData.push_back(listWidgetItemWidget);
			webCrawler->websiteLink(mod.image);
			connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
				const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
				WebCrawler* web = static_cast<WebCrawler*>(sender());
				QPixmap temp;
				if (!temp.loadFromData(data))
					qDebug() << "error loadFromData!";
				if (!temp.save(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/" + tMod->appid + "_" + tMod->id + ".png"))
					qDebug() << "error save Pixmap!" << QCoreApplication::applicationDirPath() + QString("/Temp/SteamWorkshopTool/Caches/" + tMod->appid + "_" + tMod->id + ".png");
				ListWidgetItemWidget* tListWidgetItemWidget = static_cast<ListWidgetItemWidget*>(web->otherData[1]);
				if ((tMod == nullptr) || (tListWidgetItemWidget == nullptr))
					return;
				tListWidgetItemWidget->setImage(temp);
				web->deleteLater();
				if (!webList.removeOne(web))
					qDebug() << "error removeOne web!";
				delete tMod;
				});
		}
	}
}