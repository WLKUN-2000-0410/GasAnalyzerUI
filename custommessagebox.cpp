#include "custommessagebox.h"
#include "qapplication.h"

CustomMessageBox::CustomMessageBox(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	connect(ui.pushButton_Close, &QPushButton::clicked, this, [this]() {
		done(Result::Discard);
	});
	connect(ui.toolButtonQuestion, &QPushButton::clicked, this,[this]() {
		done(Result::Confirm);
	});
	connect(ui.toolButtonClose, &QPushButton::clicked, this, [this]() {
		done(Result::CustomCancel);
	});
}

CustomMessageBox::~CustomMessageBox()
{
}

int CustomMessageBox::showCustomm(int nIcon, QString strTittle,QString strLeftButton,QString strRightButton)
{
	CustomMessageBox* messageBoxDlg = new CustomMessageBox(nullptr);
	messageBoxDlg->setIcon(nIcon);
	messageBoxDlg->setMessageString(strTittle);
	messageBoxDlg->setLeftButtonText(strLeftButton);
	messageBoxDlg->setRightButtonText(strRightButton);
	return messageBoxDlg->exec();
}

void CustomMessageBox::setIcon(int i)
{
	QPixmap pixmap(i == 0 ?QString::fromLocal8Bit(":/UiDemo/img/icon_right.png") :
		i == 1 ? QString::fromLocal8Bit(":/UiDemo/img/icon_wrong.png") :
		QString::fromLocal8Bit(":/UiDemo/img/icon_question.png"));
	ui.labelIcon->setPixmap(pixmap);
	ui.labelIcon->setScaledContents(true);
}

void CustomMessageBox::setMessageString(QString strTittle)
{
	ui.labelTitle->setText(strTittle);
}

void CustomMessageBox::setLeftButtonText(QString strLeft)
{
	ui.toolButtonQuestion->setText(strLeft);
}

void CustomMessageBox::setRightButtonText(QString strRight)
{
	ui.toolButtonClose->setText(strRight);
}
