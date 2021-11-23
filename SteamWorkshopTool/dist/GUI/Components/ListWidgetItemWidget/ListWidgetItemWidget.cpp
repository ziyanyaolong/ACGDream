#include "ListWidgetItemWidget.h"

ListWidgetItemWidget::ListWidgetItemWidget(QListWidget* parent)
	: QObject(parent)
{
	listWidgetItem = new QListWidgetItem;
	widget = new Widget1(parent);
	listWidgetItem->setSizeHint(QSize(widget->width(), widget->height()));
	parent->addItem(listWidgetItem);
	parent->setItemWidget(listWidgetItem, widget);
	connect(widget->ui.pushButton, &QPushButton::clicked, this, [&]() {
		if (widget->_isSubscription)
		{
			widget->setSubscription(false);
		}
		else
			widget->setSubscription(true);

		emit this->pushButtonResponse_Subscription(widget->_isSubscription);
		});
}

ListWidgetItemWidget::~ListWidgetItemWidget()
{
}

