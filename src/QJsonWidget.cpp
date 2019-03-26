#include "QJsonWidget.h"

#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qtextedit.h>


#include <qfilesystemmodel.h>
#include <qsplitter.h>
#include <qtreeview.h>
#include <qlistview.h>


#include <qstringlist.h>
#include <qabstractitemmodel.h>
#include <qstringlistmodel.h>

QJsonWidget::QJsonWidget(QWidget *parent, Qt::WindowFlags flags)
	: QWidget(parent, flags)
{
}

QJsonWidget::QJsonWidget(QWidget *parent, Qt::WindowFlags flags, QStringListModel *slm)
	: QWidget(parent, flags)
{

	// Unindented for quoting purposes:
	// QStringList numbers;
	// numbers << "One" << "Two" << "Three" << "Four" << "Five";
	// QAbstractItemModel *model = new QStringListModel(numbers);


	///////////////////////
	QListView *view = new QListView(this);
	view->setModel(slm);
	view->setUniformItemSizes(true);


	/*
	QGridLayout *layout = new QGridLayout(this);
	layout->setMargin(0);

	//layout->setHorizontalSpacing(2);
	//layout->setVerticalSpacing(5);
	//layout->setColumnMinimumWidth(20);
	//layout->setColumnStretch(20);
	this->setLayout(layout);

	QPushButton *button1 = new QPushButton("One");
	QPushButton *button2 = new QPushButton("Two");
	QPushButton *button3 = new QPushButton("Three");
	QPushButton *button4 = new QPushButton("Four");
	QPushButton *button5 = new QPushButton("Five");


	QTextEdit *edit1 = new QTextEdit(this);
	QTextEdit *edit2 = new QTextEdit(this);
	QTextEdit *edit3 = new QTextEdit(this);
	QTextEdit *edit4 = new QTextEdit(this);
	QTextEdit *edit5 = new QTextEdit(this);


	layout->addWidget(button1, 0, 0);
	layout->addWidget(edit1, 0, 1);
	layout->addWidget(button2, 1, 0);
	layout->addWidget(edit2, 1, 1);
	layout->addWidget(button3, 2, 0);
	layout->addWidget(edit3, 2, 1);
	layout->addWidget(button4, 3, 0);
	layout->addWidget(edit4, 3, 1);
	layout->addWidget(button5, 4, 0);
	layout->addWidget(edit5, 4, 1);*/
}


QJsonWidget::~QJsonWidget()
{
}
