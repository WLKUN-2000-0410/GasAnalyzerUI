#pragma once

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <qpropertyanimation.h>

class MessageBoxDlg : public QDialog
{
	Q_OBJECT

public:
	MessageBoxDlg(QWidget *parent = Q_NULLPTR);
	~MessageBoxDlg();
	void showMessage(const QString &msg, int timeout = 3000);
public slots:
	void hideMessage();
private:
	QLabel *label;
};
