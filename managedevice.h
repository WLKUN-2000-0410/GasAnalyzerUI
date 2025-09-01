#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_managedevice.h"


class ManageDevice : public QMainWindow
{
	Q_OBJECT

public:
	ManageDevice(QWidget *parent = Q_NULLPTR);
	~ManageDevice();

public:
	QAction* actionFileOpen, *actionFileSave, *actionFileImport
		, *actionJiao1, *actionJiao2, *actionJiao3, *actionJiao4
		,* actionScan1, *actionScan2, *actionScan3
		, *actionDevice1, *actionDevice2, *actionDevice3, *actionDevice4, *actionDevice5, *actionDevice6, *actionDevice7, *actionDevice8, *actionDevice9
		, *actionDB1, *actionDB2
		, *actionMath1, *actionMath2, *actionMath3, *actionMath4;

public slots:
	void showModeling();

protected:
	void closeEvent(QCloseEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

public:
	void initData();
	void initView();
	void initClick();
	void openFileDlg();

private:
	QPoint m_dragPosition;
	bool m_dragging = false;

private:
	Ui::ManageDevice ui;
};
