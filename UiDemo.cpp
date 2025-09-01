#include "UiDemo.h"
#include <QtMath>
#include <QShortcut>
#include <QKeyEvent>
#include <qdebug.h>
#include <qtimer.h>
#include "managedevice.h"
#include "custommessagebox.h"


UiDemo::UiDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	this->setWindowFlag(Qt::FramelessWindowHint);
	this->setFocusPolicy(Qt::StrongFocus);
	this->grabKeyboard();  // 确保捕获所有键盘输入
	initData();
	initView();
	initClick();
}

void UiDemo::keyPressEvent(QKeyEvent * event)
{

	int key = event->key();
	if (event->modifiers()==Qt::ControlModifier&&event->key()==Qt::Key_Escape)
	{
		qDebug() << "ESC";
	}
}

void UiDemo::closeEvent(QCloseEvent * event)
{
	int i = CustomMessageBox::showCustomm(3, QString::fromLocal8Bit("确认要退出吗?")
		, QString::fromLocal8Bit("是"), QString::fromLocal8Bit("否"));
	qDebug() << i;

	if (i == CustomMessageBox::Result::Confirm) {
		
	}
	else {
		event->ignore();  // 阻止关闭
	}
}

void UiDemo::initData()
{

	//qitiList.append( { "H2","O2","N2","CO2","CO","CH4","C2H6","C3H8"
	//	,"n-C4H10","i-C4H10","n-C5H12","i-C5H12",QString::fromLocal8Bit("全烃") });
	QFile inputFile("modeList/mainCheckJson.json");
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "无法打开输入文件";
		
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(inputFile.readAll(), &error);
	if (error.error != QJsonParseError::NoError) {
		qDebug() << "JSON解析错误:" << error.errorString();
		
	}

	inputFile.close();
	qitiList = doc.array();

}

void UiDemo::initView()
{
	setWindowIconText("气体拉曼");
	ui.labelShowIcon->setScaledContents(true);
	QPixmap pix(":/UiDemo/img/favicon512_512.ico");
	ui.labelShowIcon->setPixmap(pix);
	ui.widgetTitle->setVisible(false);

	ui.stackedWidget->setCurrentIndex(1);

	// 延迟加载主界面
	QTimer::singleShot(3000, [this]() {
		ui.widgetTitle->setVisible(true);
		ui.stackedWidget->setCurrentIndex(0);
		
	});
	QStringList showend;
	for (size_t i = 0; i < qitiList.size(); i++)
	{
		QJsonObject jsonObj = qitiList.at(i).toObject();
		if (jsonObj.value("checked").toBool())
		{
			showend.append(jsonObj.value("lineName").toString());
		}
	}

	for (size_t i = 0; i < showend.size(); i++)
	{
		int yu = i % 2;
		int heng = qFloor(i / 2.0);

		QSpacerItem *spacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		ui.gridLayout->addItem(spacer, heng, yu == 0 ? 0 : 4);

		QLabel* lable = new QLabel(showend.at(i));
		lable->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		ui.gridLayout->addWidget(lable, heng, yu == 0 ? 1 : 5);

		QLineEdit* d = new QLineEdit();
		d->setText("0");
		d->setFixedWidth(80);
		d->setEnabled(false);
		ui.gridLayout->addWidget(d, heng, yu == 0 ? 2 : 6);

		lable = new QLabel("%");
		ui.gridLayout->addWidget(lable, heng, yu == 0 ? 3 : 7);

		spacer = new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		ui.gridLayout->addItem(spacer, heng, yu == 0 ? 0 : 8);
	}

	
}

void UiDemo::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().ry()<30)
		{
			m_dragging = true;
			m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
	}
}

void UiDemo::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragging) {
		move(event->globalPos() - m_dragPosition);
	}
}

void UiDemo::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
}

void UiDemo::initClick()
{
	
	connect(ui.pushButtonTest, &QPushButton::clicked, this, [this]() {});
	connect(ui.pushButtonSetting, &QPushButton::clicked, this, [this]() {
		//this->close();
		ManageDevice* manageDevice = new ManageDevice();
		manageDevice->show();
	});
	connect(ui.pushButtonWidgetMin, &QPushButton::clicked, this, &UiDemo::showMinimized);
	connect(ui.pushButtonWidgetMax, &QPushButton::clicked, this, [this]() {
		isMaximized() ? showNormal() : showMaximized();
	});
	connect(ui.pushButtonWidgetClose, &QPushButton::clicked, this, &UiDemo::close);
	
}

