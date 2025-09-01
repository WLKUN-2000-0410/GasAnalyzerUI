#pragma once

#include <QWidget>
#include "ui_modeling.h"
#include <QJsonArray>

class Modeling : public QWidget
{
	Q_OBJECT

public:
	Modeling(QWidget *parent = Q_NULLPTR);
	~Modeling();

public:
	void initData();
	void initView();
	void initClick();
	void setLineNameList(QList<QString> nameList);
	void writeCheckMode(QJsonArray json);
	QJsonArray readCheckJsonByFile();
	int nTestMode = 0;
	
Q_SIGNALS:
	void setComboxIconSignals(int nIndex,QString iconPath);
	void showSmoothDialogSignale();

public slots:
	void setComboxIcon(int nIndex,QString iconPath);
	void showSmoothDialog();
	
private:
	QStringList lineNameList;
	QStringList strColor = { u8"#5470c6", u8"#91cc75", u8"#fac858", u8"#ee6666", u8"#73c0de"
		, u8"#3ba272", u8"#fc8452", u8"#9a60b4", u8"#ea7ccc" };
	QStringList folderNames;
	QJsonArray folderJsonArray;

	Ui::Modeling ui;
};
