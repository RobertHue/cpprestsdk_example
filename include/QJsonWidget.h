#pragma once

#include <qwidget.h>
#include <qstringlistmodel.h>

class QJsonWidget : public QWidget
{
	Q_OBJECT

public:
	QJsonWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0);
	QJsonWidget::QJsonWidget(QWidget *parent, Qt::WindowFlags flags, QStringListModel *slm);
	~QJsonWidget();

}; 

