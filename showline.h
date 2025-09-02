#pragma once

#include <QWidget>
#include "ui_showline.h"

class ShowLine : public QWidget
{
	Q_OBJECT

public:
	ShowLine(QWidget *parent = Q_NULLPTR);
	~ShowLine();

Q_SIGNALS:
	void addSeriesSignals(QString lineName, double* xArray, double* yArray, int length);
	void saveSelectData(QString fileName);
	void exportByFilePathSingle(QString dir);
	void importByFilePathSingle(QString dir);

public slots:
	void addSeriesVec(QString lineName, double* xArray, double* yArray, int length);
	void saveSelectDataByFileName(QString fileName);
	void exportByFilePath(QString dir);
	void importByFilePath(QString dir);

public:
	void initData();
	void initView();
	void initClick();
	void addSeries(QString lineName, std::vector<double> xList, std::vector<double> yList);
	void removeSeries(QString lineName);
	void removeSeries(int nIndex);
	void removeAllSeries();
	void updateXRange(int nReduce, double range);
	void updateYRange(int nReduce, double range);
	void renameByLine(int nIndex, QString lineName);
	QString getSeriesName();
	
	void isVisbleLine(int nIndex, bool isHidden);
	QList<int> getSelected();
	QList<int> getUnSelected();

	QList<QJsonObject> getSelectedByObj();
	QList<QJsonObject> getUnSelectedByObj();

	QJsonObject getDataByLineName(QString lineName);
	QList<QJsonObject> getAllDataList();
	

public:
	int nIndex = 1;
	QList<QString> lineNames;
	QStringList strColor = { u8"#5470c6", u8"#91cc75", u8"#fac858", u8"#ee6666", u8"#73c0de"
		, u8"#3ba272", u8"#fc8452", u8"#9a60b4", u8"#ea7ccc" };
	float rangeVal = 0.2;
	QList<QJsonObject> lineObjList;

private:
	Ui::ShowLine ui;
};
