#include "Widget1.h"

QIcon Widget1::YSIcon;
QIcon Widget1::NSIcon;

Widget1::Widget1(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.name->setWordWrap(true);
	
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
	_image = _image.scaled(QSize(ui.cover->width(), ui.cover->height()));
	ui.cover->setPixmap(_image);
}

void Widget1::setImage(const QString& image)
{
	if (!_image.load(image))
		return;
	_image = _image.scaled(QSize(ui.cover->width(), ui.cover->height()));
	ui.cover->setPixmap(_image);
}

void Widget1::setStar(Star star)
{
	switch (star)
	{
	case Widget1::Star::Zero:

		break;
	case Widget1::Star::One:
		break;
	case Widget1::Star::Two:
		break;
	case Widget1::Star::Three:
		break;
	case Widget1::Star::Four:
		break;
	case Widget1::Star::Five:
		break;
	default:
		break;
	}
}
