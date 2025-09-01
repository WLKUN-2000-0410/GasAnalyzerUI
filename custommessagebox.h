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
		Confirm = 1,      // ȷ�ϲ���
		Save = 2,         // �������
		Discard = 3,      // ��������
		CustomCancel = 4  // �Զ���ȡ��
	};
	static int showCustomm(int nIcon, QString strTittle,QString strLeft,QString strRight);
	void setIcon(int i);
	void setMessageString(QString strTittle);
	void setLeftButtonText(QString strLeft);
	void setRightButtonText(QString strRight);
private:
	Ui::CustomMessageBox ui;
};
