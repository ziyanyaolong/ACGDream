#pragma once

#include <QWidget>
#include "ui_ItemWidgetStyle0.h"

namespace BlockItemWidgetStyle
{
	class ItemWidgetStyle0 : public QWidget
	{
		Q_OBJECT

	public:
		explicit ItemWidgetStyle0(QWidget* parent = Q_NULLPTR);
		virtual ~ItemWidgetStyle0();

		void setPixmap(const QPixmap& pixmap);

	private:
		Ui::ItemWidgetStyle0Class ui;

	};
}