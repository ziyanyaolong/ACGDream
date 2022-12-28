#include "SWTUI.h"
	//border-top:0px solid #e8f3f9;background: 
	// transparent;
SWTUI::SWTUI(QWidget *parent)
	: QWidget(parent)
{
	this->metaTypeRegister();

	ui.setupUi(this);

	this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);//无边框
	this->setAttribute(Qt::WA_TranslucentBackground, true);//窗体背景全透明
	//ui.tab->setAutoFillBackground(true);
	//QPalette palette = ui.tab->palette();
	//palette.setColor(QPalette::Background, QColor(100, 100, 100, 0));
	//ui.tab->setPalette(palette);

	ui.listWidget->setResizeMode(QListView::Adjust);

	ui.listWidget_2->setResizeMode(QListView::Adjust);

	ui.textBrowser->insertPlainText(QString::fromUtf8("创意工坊工具启动完成\n"));
	delayPushW = new QTimer(this);
	delayPushL = new QTimer(this);

	connect(delayPushW, &QTimer::timeout, this, [&]() {
		delayPushW->stop();
		clearModList(SWTUI::ListWay::Website);
		QStringList datalist;
		datalist.clear();
		datalist.push_back(ui.lineEdit->text());
		datalist.push_back(ui.lineEdit_Search->text());
		datalist.push_back(ui.label_page->text());
		emit this->loadList(datalist, SWTUI::ListWay::Website);
		});

	connect(delayPushL, &QTimer::timeout, this, [&]() {
		delayPushL->stop();
		clearModList(SWTUI::ListWay::Local);
		QStringList datalist;
		datalist.clear();
		datalist.push_back(ui.lineEdit_2->text());
		datalist.push_back(ui.lineEdit_Search_2->text());
		datalist.push_back(ui.label_page_2->text());
		emit this->loadList(datalist, SWTUI::ListWay::Local);
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

	connect(ui.commandLinkButton_ClearCache, &QCommandLinkButton::clicked, this, &SWTUI::clearCache);

	connect(ui.commandLinkButton_Update, &QCommandLinkButton::clicked, this, &SWTUI::updateMod);

	connect(ui.commandLinkButton_StartSteamCMD, &QPushButton::clicked, this, &SWTUI::startSteamCMD);

	addEventFilterAllWidget();
}

SWTUI::~SWTUI()
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

	clearModList(SWTUI::ListWay::All);
}

void SWTUI::messageBox(const QString& title, const QString& text, QMessageBox::Icon icon, const QVector<QMessageBoxButtonData>& pushButtonList)
{
	QMessageBox box(icon, title, text);

	box.setParent(this);
	box.setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

	QPushButton* pushButtonPointer = nullptr;
	qint64 id = 0;
	foreach(auto && i, pushButtonList)
	{
		pushButtonPointer = box.addButton(i.str, i.buttonRole);
		if (i.isEmitSignal)
		{
			if (i.receiver == nullptr)
			{
				emit this->messageBoxButtonConnect(sender(), id, pushButtonPointer);
			}
			else
			{
				emit this->messageBoxButtonConnect(i.receiver, id, pushButtonPointer);
			}
		}
		id++;
	}

	box.show();
	box.exec();
}

void SWTUI::clearModList(SWTUI::ListWay way)
{
	switch (way)
	{
	case SWTUI::ListWay::Website:
		emit this->clearWebsiteList();
		ui.listWidget->clear();
		break;
	case SWTUI::ListWay::Local:
		emit this->clearLocalList();
		ui.listWidget_2->clear();
		break;

	case SWTUI::ListWay::All:
		emit this->clearLocalList();
		emit this->clearWebsiteList();
		ui.listWidget->clear();
		ui.listWidget_2->clear();
		break;

	default:
		break;
	}
	
}

void SWTUI::addMod(const ModDataTable& mod, SWTUI::ListWay into)
{
	ListWidgetItemWidget* listWidgetItemWidget = nullptr;
	switch (into)
	{
	case SWTUI::ListWay::Website:
		listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget);
		break;

	case SWTUI::ListWay::Local:
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
			emit this->subscription(isSubscription, temp->readWidget()->readId(), SWTUI::ListWay::Website);
		}
		else
		{
			emit this->subscription(isSubscription, temp->readWidget()->readId(), SWTUI::ListWay::Local);
		}
		});

	emit this->addModReturn(listWidgetItemWidget, mod, into);
}

void SWTUI::refresh(SWTUI::ListWay way)
{
	switch (way)
	{
	case SWTUI::ListWay::Website:
		ui.pushButton->clicked();
		break;

	case SWTUI::ListWay::Local:
		ui.pushButton_2->clicked();
		break;

	case SWTUI::ListWay::All:
		ui.pushButton->clicked();
		ui.pushButton_2->clicked();
		break;

	default:
		break;
	}
}

void SWTUI::addEventFilterAllWidget()
{
	auto list = findChildren<QObject*>();
	foreach(QObject * i, list)
	{
		i->installEventFilter(this);
	}
}

//获取子控件的所有消息
bool SWTUI::eventFilter(QObject* target, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
		if (keyEvent)
		{
			if (keyEvent->key() == QEvent::Enter)
			{
				if (target == ui.lineEdit_Search)
				{
					emit ui.pushButton_Search->clicked();
				}
				else if (target == ui.lineEdit_Search_2)
				{
					emit ui.pushButton_Search_2->clicked();
				}
			}
		}
	}

	//其他消息返回给基类消息监控器处理
	return QWidget::eventFilter(target, event);
}

void SWTUI::metaTypeRegister()
{
	qRegisterMetaType<SWTUI::QMessageBoxButtonData>("SWTUI::QMessageBoxButtonData");
	qRegisterMetaType<QVector<SWTUI::QMessageBoxButtonData>>("QVector<SWTUI::QMessageBoxButtonData>");
	qRegisterMetaType<QVector<SWTUI::QMessageBoxButtonData>>("QVector<SWTUI::QMessageBoxButtonData>&");
	qRegisterMetaType<QMessageBox::Icon>("QMessageBox::Icon");
	qRegisterMetaType<QMessageBox::StandardButton>("QMessageBox::StandardButton");
	qRegisterMetaType<SWTUI::Form>("SWTUI::Form");
	qRegisterMetaType<SWTUI::ListWay>("SWTUI::ListWay");
}
