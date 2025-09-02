#pragma once

#include <QWidget>
#include "ui_listitemwidget.h"

class ListItemWidget : public QWidget
{
	Q_OBJECT

Q_SIGNALS:
	void updatePix(int nIndex,QString strWave);

public:
	ListItemWidget(QWidget *parent = Q_NULLPTR);
	~ListItemWidget();
	void setWave(QString strWave);
	void setPix(QString strPwd);
	void setIndex(int nIndex);
	QString getWave();
	QString getPix();
	int getIndex();

private:
	int nIndex = 0;
	Ui::ListItemWidget ui;
};
