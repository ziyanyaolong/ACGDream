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
		QStringList datalist;
		datalist.clear();
		datalist.push_back(ui.lineEdit->text());
		datalist.push_back(ui.lineEdit_Search->text());
		datalist.push_back(ui.label_page->text());
		emit this->loadList(datalist, SWTGUI::ListWay::Website);
		});

	connect(delayPushL, &QTimer::timeout, this, [&]() {
		delayPushL->stop();
		clearModList(SWTGUI::ListWay::Local);
		QStringList datalist;
		datalist.clear();
		datalist.push_back(ui.lineEdit_2->text());
		datalist.push_back(ui.lineEdit_Search_2->text());
		datalist.push_back(ui.label_page_2->text());
		emit this->loadList(datalist, SWTGUI::ListWay::Local);
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

	addEventFilterAllWidget();
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
		emit this->clearWebsiteList();
		ui.listWidget->clear();
		break;
	case SWTGUI::ListWay::Local:
		emit this->clearLocalList();
		ui.listWidget_2->clear();
		break;

	case SWTGUI::ListWay::All:
		emit this->clearLocalList();
		emit this->clearWebsiteList();
		ui.listWidget->clear();
		ui.listWidget_2->clear();
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
		listWidgetItemWidget = new ListWidgetItemWidget(ui.listWidget);
		break;

	case SWTGUI::ListWay::Local:
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

	emit this->addModReturn(listWidgetItemWidget, mod, into);
}

void SWTGUI::newItemWidget(const ModDataTable& mod)
{
	auto list = findChildren<QObject*>();
	foreach(QObject * i, list)
	{
		i->installEventFilter(this);
	}
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

//获取子控件的所有消息
bool SWTGUI::eventFilter(QObject* target, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
	}

	//其他消息返回给基类消息监控器处理
	return QWidget::eventFilter(target, event);
}