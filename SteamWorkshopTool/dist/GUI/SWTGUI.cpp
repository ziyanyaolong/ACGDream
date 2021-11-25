#include "SWTGUI.h"

SWTGUI::SWTGUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowOpacity(0.7);
	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	delayPushW = new QTimer(this);
	delayPushL = new QTimer(this);

	qRegisterMetaType<QMessageBox::StandardButton>("QMessageBox::StandardButton");
	qRegisterMetaType<SWTGUI::Form>("SWTGUI::Form");
	qRegisterMetaType<SWTGUI::ListWay>("SWTGUI::ListWay");

	connect(delayPushW, &QTimer::timeout, this, [&]() {
		delayPushW->stop();
		clearModList(SWTGUI::ListWay::Website);
		webModSaveData.datalist.clear();
		webModSaveData.datalist.push_back(ui.lineEdit->text());
		webModSaveData.datalist.push_back(ui.lineEdit_Search->text());
		webModSaveData.datalist.push_back(ui.label_page->text());
		emit this->loadList(webModSaveData.datalist, SWTGUI::ListWay::Website);
		});

	connect(delayPushL, &QTimer::timeout, this, [&]() {
		delayPushL->stop();
		clearModList(SWTGUI::ListWay::Local);
		localModSaveData.datalist.clear();
		localModSaveData.datalist.push_back(ui.lineEdit_2->text());
		localModSaveData.datalist.push_back(ui.lineEdit_Search_2->text());
		localModSaveData.datalist.push_back(ui.label_page_2->text());
		emit this->loadList(localModSaveData.datalist, SWTGUI::ListWay::Local);
		});

	connect(ui.pushButton, &QPushButton::clicked, this, [&]() {
		delayPushW->start(300);
		});

	connect(ui.pushButton_Previous, &QPushButton::clicked, this, [&]() {
		if((ui.label_page->text().toULongLong() - 1) == 1)
			ui.pushButton_Previous->setEnabled(false);
		ui.label_page->setText(QString::number(ui.label_page->text().toULongLong() - 1));
		delayPushW->start(300);
		});

	connect(ui.pushButton_Next, &QPushButton::clicked, this, [&]() {
		if (!ui.pushButton_Previous->isEnabled())
			ui.pushButton_Previous->setEnabled(true);
		ui.label_page->setText(QString::number(ui.label_page->text().toULongLong() + 1));
		delayPushW->start(300);
		});

	connect(ui.pushButton_Search, &QPushButton::clicked, this, [&]() {
		ui.label_page->setText(QString::number(1));
		delayPushW->start(300);
		});

	connect(ui.pushButton_2, &QPushButton::clicked, this, [&]() {
		delayPushL->start(300);
		});

	connect(ui.pushButton_Previous_2, &QPushButton::clicked, this, [&]() {
		if ((ui.label_page_2->text().toULongLong() - 1) == 1)
			ui.pushButton_Previous_2->setEnabled(false);
		ui.label_page_2->setText(QString::number(ui.label_page_2->text().toULongLong() - 1));
		delayPushL->start(300);
		});

	connect(ui.pushButton_Next_2, &QPushButton::clicked, this, [&]() {
		if (!ui.pushButton_Previous_2->isEnabled())
			ui.pushButton_Previous_2->setEnabled(true);
		ui.label_page_2->setText(QString::number(ui.label_page_2->text().toULongLong() + 1));
		delayPushL->start(300);
		});

	connect(ui.pushButton_Search_2, &QPushButton::clicked, this, [&]() {
		ui.label_page_2->setText(QString::number(1));
		delayPushL->start(300);
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
	if (delayPushW)
	{
		delayPushW->stop();
		delayPushW->deleteLater();
	}
	
	if (delayPushL)
	{
		delayPushL->stop();
		delayPushL->deleteLater();
	}

	clearModList(SWTGUI::ListWay::All);
}

void SWTGUI::messageBox(SWTGUI::Form form, const QString& title, const QString& text, QMessageBox::StandardButton button0, QMessageBox::StandardButton button1)
{
	switch (form)
	{
	case SWTGUI::Form::Critical:
		QMessageBox::critical(this, title, text, button0, button1);
		break;

	case SWTGUI::Form::Information:
		QMessageBox::information(this, title, text, button0, button1);
		break;

	case SWTGUI::Form::Question:
		QMessageBox::question(this, title, text, button0, button1);
		break;

	case SWTGUI::Form::Warning:
		QMessageBox::warning(this, title, text, button0, button1);
		break;

	case SWTGUI::Form::About:
		QMessageBox::about(this, title, text);
		break;

	default:
		break;
	}
}

void SWTGUI::clearModList(SWTGUI::ListWay way)
{
	switch (way)
	{
	case SWTGUI::ListWay::Website:
		ui.listWidget->clear();
		foreach(auto i, webModSaveData.webList)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		foreach(auto i, webModSaveData.list)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		webModSaveData.list.clear();
		webModSaveData.webList.clear();
		break;
	case SWTGUI::ListWay::Local:
		ui.listWidget_2->clear();
		foreach(auto i, localModSaveData.webList)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		foreach(auto i, localModSaveData.list)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		localModSaveData.list.clear();
		localModSaveData.webList.clear();
		break;

	case SWTGUI::ListWay::All:
		ui.listWidget_2->clear();
		ui.listWidget->clear();
		foreach(auto i, webModSaveData.webList)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		foreach(auto i, webModSaveData.list)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		foreach(auto i, localModSaveData.webList)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		foreach(auto i, localModSaveData.list)
		{
			if (i != nullptr)
			{
				disconnect(i, 0, 0, 0);
				i->deleteLater();
			}
		}
		localModSaveData.list.clear();
		localModSaveData.webList.clear();
		webModSaveData.list.clear();
		webModSaveData.webList.clear();
		break;

	default:
		break;
	}
	
}

void SWTGUI::addMod(const ModDataTable& mod, SWTGUI::ListWay into)
{
	ListWidgetItemWidget* listWidgetItemWidget = nullptr;
	switch (into)
	{
	case SWTGUI::ListWay::Website:
		webModSaveData.list.push_back(listWidgetItemWidget);
		listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget);
		break;

	case SWTGUI::ListWay::Local:
		localModSaveData.list.push_back(listWidgetItemWidget);
		listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget_2);
		break;

	default:
		return;
		break;
	}
	
	connect(listWidgetItemWidget, &ListWidgetItemWidget::pushButtonResponse_Subscription, this, [&](bool isSubscription) {
		ListWidgetItemWidget* temp = static_cast<ListWidgetItemWidget*>(sender());
		if (temp->parent() == ui.listWidget)
		{
			emit this->subscription(isSubscription, temp->readWidget()->readId(), SWTGUI::ListWay::Website);
		}
		else
		{
			emit this->subscription(isSubscription, temp->readWidget()->readId(), SWTGUI::ListWay::Local);
		}
		}); 
	
	listWidgetItemWidget->readWidget()->setTitle(mod.title);
	listWidgetItemWidget->readWidget()->setId(QString(mod.appid + mod.id));
	listWidgetItemWidget->readWidget()->setSubscription(mod.isSubscribe);

	if (mod.image != "")
	{
		QPixmap pixmap0;
		if (pixmap0.load(QString(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches/" + mod.appid + "_" + mod.id + ".png")))
		{
			listWidgetItemWidget->readWidget()->setImage(pixmap0);
		}
		else
		{
			QDir dir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
			if (!dir.exists())
				dir.mkdir(QCoreApplication::applicationDirPath() + "/Temp/SteamWorkshopTool/Caches");
			WebCrawler* webCrawler = new WebCrawler(this);
			switch (into)
			{
			case SWTGUI::ListWay::Website:
				webModSaveData.webList.push_back(webCrawler);
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
					tListWidgetItemWidget->readWidget()->setImage(temp);
					if (!webModSaveData.webList.removeOne(web))
						qDebug() << "删除错误";
					delete tMod;
					web->deleteLater();
					});
				break;

			case SWTGUI::ListWay::Local:
				localModSaveData.webList.push_back(webCrawler);
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
					tListWidgetItemWidget->readWidget()->setImage(temp);
					if (!localModSaveData.webList.removeOne(web))
						qDebug() << "删除错误";
					delete tMod;
					web->deleteLater();
					});
				break;

			default:
				break;
			}
			webCrawler->otherData.push_back(new ModDataTable(mod));
			webCrawler->otherData.push_back(listWidgetItemWidget);
			webCrawler->websiteLink(mod.image);
		}
	}

}

void SWTGUI::newItemWidget(const ModDataTable& mod)
{

}

void SWTGUI::refresh(SWTGUI::ListWay way)
{
	switch (way)
	{
	case SWTGUI::ListWay::Website:
		ui.pushButton->clicked();
		break;

	case SWTGUI::ListWay::Local:
		ui.pushButton_2->clicked();
		break;

	case SWTGUI::ListWay::All:
		ui.pushButton->clicked();
		ui.pushButton_2->clicked();
		break;

	default:
		break;
	}
}
