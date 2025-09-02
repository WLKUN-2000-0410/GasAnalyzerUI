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
	QList<QString> getLineNameList();
	void setLaserPwd(int nLaserPwd);
	QList<QJsonObject> getAllDataList();

Q_SIGNALS:
	void addSeriesSignals(QString lineName, std::vector<double> xArray, std::vector<double> yArray);
	void deleteChoseLine();
	void renameLine(int nIndex,QString strNewLineName);
	void importByFilePath(QString dir);
	void exportByFilePath(QString dir);
	void saveLineByFine(QString fileName);
	

public slots:
	void singleScan();
	void startScan();
	void stopScan();

private:
	Ui::ScanTestDlg ui;
};
