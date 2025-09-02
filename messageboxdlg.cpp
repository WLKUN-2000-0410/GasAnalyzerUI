#include "messageboxdlg.h"
#include "qgridlayout.h"

MessageBoxDlg::MessageBoxDlg(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlag(Qt::FramelessWindowHint);
	label = new QLabel();
	label->setAlignment(Qt::AlignCenter);
	QGridLayout* grid = new QGridLayout(this);
	grid->addWidget(label);
	this->raise();
}

MessageBoxDlg::~MessageBoxDlg()
{
	delete label;
}

void MessageBoxDlg::showMessage(const QString & msg, int timeout)
{
	label->setText(msg);
	show();
	QTimer *timer = new QTimer(this);
	timer->setSingleShot(true);
	timer->setInterval(timeout);
	connect(timer, &QTimer::timeout, this,[this](){
		QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
		fadeOut->setDuration(1000);
		fadeOut->setStartValue(1);
		fadeOut->setEndValue(0);
		fadeOut->start();
		connect(fadeOut, &QPropertyAnimation::finished, this, &MessageBoxDlg::hideMessage);
	});
	timer->start(2000);
}

void MessageBoxDlg::hideMessage() {
	this->close();
}
