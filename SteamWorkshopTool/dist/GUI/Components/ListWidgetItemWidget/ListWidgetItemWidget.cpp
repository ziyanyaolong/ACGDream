#include "ListWidgetItemWidget.h"

QIcon ListWidgetItemWidget::YSIcon;
QIcon ListWidgetItemWidget::NSIcon;

ListWidgetItemWidget::ListWidgetItemWidget(QWidget *parent, QListWidgetItem* listWidgetItem)
	: QWidget(parent), _listWidgetItem(listWidgetItem)
{
	ui.setupUi(this);
	QPixmap tImage = QPixmap(":/SteamWorkshopTool/assets/Images/NSubscription.png");
	tImage = tImage.scaled(QSize(ui.pushButton->width(), ui.pushButton->height()));
	NSIcon = QIcon(tImage);

	tImage = QPixmap(":/SteamWorkshopTool/assets/Images/YSubscription.png");
	tImage = tImage.scaled(QSize(ui.pushButton->width(), ui.pushButton->height()));
	YSIcon = QIcon(tImage);
	ui.pushButton->setIcon(NSIcon);
	
	connect(ui.pushButton, &QPushButton::clicked, this, [&]() {
		if (_isSubscription)
		{
			setSubscription(false);
		}
		else
			setSubscription(true);

		pushButtonResponse_Subscription(_isSubscription);
		});
}

ListWidgetItemWidget::~ListWidgetItemWidget()
{
}

void ListWidgetItemWidget::setSubscription(bool subscription)
{
	if (subscription)
	{
		ui.pushButton->setIcon(YSIcon);
		_isSubscription = true;
	}
	else
	{
		ui.pushButton->setIcon(NSIcon);
		_isSubscription = false;
	}
}