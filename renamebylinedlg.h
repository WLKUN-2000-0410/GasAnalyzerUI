#pragma once

#include <QWidget>
#include "ui_renamebylinedlg.h"
#include <QMouseEvent>
#include "scantestdlg.h"

class RenameByLineDlg : public QWidget
{
	Q_OBJECT

public:
	RenameByLineDlg(QWidget *parent = Q_NULLPTR);
	~RenameByLineDlg();

	void initData();
	void initView();
	void initClick();
	void setLineNameList(QList<QString> nameList);
	ScanTestDlg* scanTestDlg;

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QPoint m_dragPosition;
	bool m_dragging = false;

private:
	Ui::RenameByLineDlg ui;
};
