#pragma once

#include <QDialog>
#include "ui_smoothdialog.h"

class SmoothDialog : public QDialog
{
	Q_OBJECT

public:
	SmoothDialog(QWidget *parent = Q_NULLPTR);
	~SmoothDialog();

	void initData();
	void initView();
	void initClick();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QPoint m_dragPosition;
	bool m_dragging = false;

private:
	Ui::SmoothDialog ui;
};
