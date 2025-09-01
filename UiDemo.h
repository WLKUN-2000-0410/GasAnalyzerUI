#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UiDemo.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class UiDemo : public QMainWindow
{
    Q_OBJECT

public:
    UiDemo(QWidget *parent = Q_NULLPTR);


protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	QPoint m_dragPosition;
	bool m_dragging = false;

public:
	QJsonArray qitiList;
	void initData();
	void initView();
	void initClick();
	

private:
    Ui::UiDemoClass ui;
};
