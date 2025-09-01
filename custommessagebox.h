#pragma once

#include <QDialog>
#include "ui_custommessagebox.h"

class CustomMessageBox : public QDialog
{
	Q_OBJECT

public:
	CustomMessageBox(QWidget *parent = Q_NULLPTR);
	~CustomMessageBox();

public:
	enum Result {
		Confirm = 1,      // 确认操作
		Save = 2,         // 保存操作
		Discard = 3,      // 丢弃操作
		CustomCancel = 4  // 自定义取消
	};
	static int showCustomm(int nIcon, QString strTittle,QString strLeft,QString strRight);
	void setIcon(int i);
	void setMessageString(QString strTittle);
	void setLeftButtonText(QString strLeft);
	void setRightButtonText(QString strRight);
private:
	Ui::CustomMessageBox ui;
};
