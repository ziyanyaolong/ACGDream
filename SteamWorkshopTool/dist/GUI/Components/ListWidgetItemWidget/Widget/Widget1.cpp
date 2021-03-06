#include "Widget1.h"

QIcon Widget1::YSIcon;
QIcon Widget1::NSIcon;

Widget1::Widget1(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	if (NSIcon.isNull())
	{
		QPixmap tImage = QPixmap(":/SteamWorkshopTool/assets/Images/NSubscription.png");
		tImage = tImage.scaled(QSize(ui.pushButton->width(), ui.pushButton->height()));
		NSIcon = QIcon(tImage);
	}
	
	if (YSIcon.isNull())
	{
		QPixmap tImage = QPixmap(":/SteamWorkshopTool/assets/Images/YSubscription.png");
		tImage = tImage.scaled(QSize(ui.pushButton->width(), ui.pushButton->height()));
		YSIcon = QIcon(tImage);
	}
		

	ui.pushButton->setIcon(NSIcon);
}

Widget1::~Widget1()
{
}

void Widget1::setSubscription(bool subscription)
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

void Widget1::setImage(const QPixmap& image)
{
	_image = image;
	_image = _image.scaled(QSize(ui.label->width(), ui.label->height()));
	ui.label->setPixmap(_image);
}

void Widget1::setImage(const QString& image)
{
	if (!_image.load(image))
		return;
	_image = _image.scaled(QSize(ui.label->width(), ui.label->height()));
	ui.label->setPixmap(_image);
}