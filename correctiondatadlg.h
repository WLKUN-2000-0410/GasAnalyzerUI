#pragma once

#include <QDialog>
#include "ui_correctiondatadlg.h"

class CorrectionDataDlg : public QDialog
{
	Q_OBJECT

public:
	CorrectionDataDlg(QWidget *parent = Q_NULLPTR);
	~CorrectionDataDlg();

	void initData();
	void initView();
	void initClick();
	void setLineObjList(QList<QJsonObject> lineList);
	void readInit();
	bool copyFileToPath(const QString &sourcePath, const QString &destPath, bool overwrite = false);

Q_SIGNALS:
	void updateListByIndexSingle(int nIndex);
	void initTableViewSingle();

public slots:
	void updateListByIndex(int nIndex);
	void initTableView();
	void updatePix(int nIndex,QString strWave);
	void correction();
	void exportNumIni();
	void autoFill();
	void laserCorrection();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QPoint m_dragPosition;
	bool m_dragging = false;
	QList<QJsonObject> lineObjList;
	QStringList strColor = { u8"#5470c6", u8"#91cc75", u8"#fac858", u8"#ee6666", u8"#73c0de"
		, u8"#3ba272", u8"#fc8452", u8"#9a60b4", u8"#ea7ccc" };
	QJsonObject waveObj;

private:
	Ui::CorrectionDataDlg ui;
};
