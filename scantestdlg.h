#pragma once

#include <QWidget>
#include "ui_scantestdlg.h"

class ScanTestDlg : public QWidget
{
	Q_OBJECT

public:
	ScanTestDlg(QWidget *parent = Q_NULLPTR);
	~ScanTestDlg();

	void initData();
	void initView();
	void initClick();

private:
	Ui::ScanTestDlg ui;
};
