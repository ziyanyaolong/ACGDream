#include "SWTGUI.h"

SWTGUI::SWTGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	connect(ui.pushButton, &QPushButton::clicked, this, [&]() {
		datalist.clear();
		datalist.push_back(ui.lineEdit->text());
		datalist.push_back(ui.lineEdit_Search->text());
		datalist.push_back(ui.label_page->text());
		emit this->pushButtonResponse_WebAddress(datalist);
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
}

SWTGUI::~SWTGUI()
{
	clearModList();
}

void SWTGUI::clearModList()
{
	foreach (auto i, list)
	{
		disconnect(i, 0, 0, 0);
		delete i;
	}
	ui.listWidget->clear();
	list.clear();
}

void SWTGUI::addMod(const ModDataTable& mod)
{
	QListWidgetItem* listWidgetItem = new QListWidgetItem;
	ListWidgetItemWidget* listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget, listWidgetItem);
	connect(listWidgetItemWidget, &ListWidgetItemWidget::pushButtonResponse_Subscription, this, [&](bool isSubscription) {
		ListWidgetItemWidget* temp = static_cast<ListWidgetItemWidget*>(sender());
		emit this->pushButtonResponse_Subscription(isSubscription, temp->readId());
		}); 
	list.push_back(listWidgetItemWidget);
	listWidgetItem->setSizeHint(QSize(512, 128));
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
			connect(webCrawler, &WebCrawler::finished, this, [&](const QByteArray& data) {
				const ModDataTable* tMod = static_cast<const ModDataTable*>((static_cast<WebCrawler*>(sender()))->otherData[0]);
				WebCrawler* web = static_cast<WebCrawler*>(sender());
				ListWidgetItemWidget* tListWidgetItemWidget = static_cast<ListWidgetItemWidget*>(web->otherData[1]);
				if ((tMod == nullptr) || (tListWidgetItemWidget == nullptr))
					return;
				QPixmap temp;
				if (!temp.loadFromData(data))
					qDebug() << "error loadFromData!";
				if (!temp.save(QCoreApplication::applicationDirPath() + "/Caches/" + tMod->appid + "_" + tMod->id + ".png"))
					qDebug() << "error save Pixmap!" << QCoreApplication::applicationDirPath() + QString("/Caches/" + tMod->appid + "_" + tMod->id + ".png");
				delete tMod;
				tListWidgetItemWidget->setImage(temp);
				web->deleteLater();
				});
		}
	}
}