#include "managedevice.h"
#include "qmenu.h"
#include "showline.h"
#include "modeling.h"
#include "custommessagebox.h"
#include "scantestdlg.h"


ManageDevice::ManageDevice(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowFlag(Qt::FramelessWindowHint);


//	initQCustomPlot(ui.stackedWidget);
	initData();
	initView();
	initClick();
}

ManageDevice::~ManageDevice()
{
}

void ManageDevice::closeEvent(QCloseEvent * event)
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

void ManageDevice::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().ry()<30)
		{
			m_dragging = true;
			m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
	}
}

void ManageDevice::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragging) {
		move(event->globalPos() - m_dragPosition);
	}
}

void ManageDevice::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
}

void ManageDevice::initData()
{

}

void ManageDevice::initView()
{
	ScanTestDlg* scanTestDlg = new ScanTestDlg(this);
	ui.stackedWidget->insertWidget(0, scanTestDlg);

	Modeling* modeling = new Modeling(this);
	ui.stackedWidget->insertWidget(1, modeling);

	ui.stackedWidget->setCurrentIndex(0);

	QMenu* menuFile = new QMenu(this);
	actionFileOpen = menuFile->addAction(QString::fromLocal8Bit("打开"));
	actionFileSave = menuFile->addAction(QString::fromLocal8Bit("保存"));
	actionFileImport = menuFile->addAction(QString::fromLocal8Bit("导出"));
	ui.toolButton_file->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_file->setMenu(menuFile);

	QMenu* menujiao = new QMenu(this);
	actionJiao1 = menujiao->addAction(QString::fromLocal8Bit("校正"));
	actionJiao2 = menujiao->addAction(QString::fromLocal8Bit("参考探测器"));
	actionJiao3 = menujiao->addAction(QString::fromLocal8Bit("校准"));
	actionJiao4 = menujiao->addAction(QString::fromLocal8Bit("气路清洁"));
	ui.toolButton_jiaozheng->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_jiaozheng->setMenu(menujiao);

	QMenu* menuScan = new QMenu(this);
	actionScan1 = menuScan->addAction(QString::fromLocal8Bit("探测器"));
	actionScan2 = menuScan->addAction(QString::fromLocal8Bit("激光器"));
	actionScan3 = menuScan->addAction(QString::fromLocal8Bit("采集"));
	ui.toolButton_scan->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_scan->setMenu(menuScan);

	QMenu* menuDevice = new QMenu(this);
	actionDevice1 = menuDevice->addAction(QString::fromLocal8Bit("探测器"));
	actionDevice2 = menuDevice->addAction(QString::fromLocal8Bit("激光器"));
	actionDevice3 = menuDevice->addAction(QString::fromLocal8Bit("进气阀"));
	actionDevice4 = menuDevice->addAction(QString::fromLocal8Bit("温度计"));
	actionDevice5 = menuDevice->addAction(QString::fromLocal8Bit("压力计"));
	actionDevice6 = menuDevice->addAction(QString::fromLocal8Bit("进气阀"));
	actionDevice7 = menuDevice->addAction(QString::fromLocal8Bit("真空泵"));
	actionDevice8 = menuDevice->addAction(QString::fromLocal8Bit("氖灯"));
	actionDevice9 = menuDevice->addAction(QString::fromLocal8Bit("排气阀"));
	ui.toolButton_device->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_device->setMenu(menuDevice);

	QMenu* menuDB = new QMenu(this);
	actionDB1 = menuDB->addAction(QString::fromLocal8Bit("建模"));
	actionDB2 = menuDB->addAction(QString::fromLocal8Bit("查看建模"));
	ui.toolButton_db->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_db->setMenu(menuDB);

	QMenu* menuMath = new QMenu(this);
	actionMath1 = menuMath->addAction(QString::fromLocal8Bit("谱线拟合"));
	actionMath2 = menuMath->addAction(QString::fromLocal8Bit("谱线标峰"));
	actionMath3 = menuMath->addAction(QString::fromLocal8Bit("谱线平滑"));
	actionMath4 = menuMath->addAction(QString::fromLocal8Bit("峰面积、半高宽"));
	ui.toolButton_math->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_math->setMenu(menuMath);
}

void ManageDevice::initClick()
{
	connect(actionFileOpen	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionFileSave	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionFileImport, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionJiao1		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionJiao2		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionJiao3, &QAction::triggered, this, [this]() {
		
	});
	connect(actionJiao4		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionScan1		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionScan2		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionScan3, &QAction::triggered, this, [this]() {
		ui.stackedWidget->setCurrentIndex(0);
	});
	connect(actionDevice1	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice2	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice3	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice4	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice5	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice6	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice7	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice8	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDevice9	, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionDB1		, &QAction::triggered, this, &ManageDevice::showModeling);
	connect(actionDB2		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionMath1		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionMath2		, &QAction::triggered, this, &ManageDevice::openFileDlg);
	connect(actionMath3		, &QAction::triggered, this, &ManageDevice::openFileDlg);

	connect(ui.pushButtonWidgetMin, &QPushButton::clicked, this, &ManageDevice::showMinimized);
	connect(ui.pushButtonWidgetMax, &QPushButton::clicked, this, [this]() {
		isMaximized() ? showNormal() : showMaximized();
	});
	connect(ui.pushButtonWidgetClose, &QPushButton::clicked, this, &ManageDevice::close);
}

void ManageDevice::openFileDlg()
{
	QString fileName = QFileDialog::getOpenFileName(
		this,
		QString::fromLocal8Bit("选择文件"),
		"",
		QString::fromLocal8Bit("文本文件 (*.txt);")
	);
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // 文本模式读取
															 // 操作文件
		QByteArray data = file.readAll();
		QString strFileC = QString::fromUtf8(data);
		QFileInfo fileInfo(fileName);
		QString fileNameA = fileInfo.fileName().replace(".ini", "").replace(".txt", "");
		QStringList lines = strFileC.split(QString::fromLocal8Bit("\n"));
		QString line = lines.at(0);
		int nlineCount = line.split(QString::fromLocal8Bit("\t")).size() / 2;
		if (lines.size()>0)
		{
			std::vector<double> xVec;
			std::vector<double> yVec;


			for (size_t i = 0; i < nlineCount; i++)
			{
				for each (QString strLine in lines)
				{
					QStringList strLineArray = strLine.split(QString::fromLocal8Bit("\t"));
					if (strLineArray.size() >= 2)
					{
						if (strLineArray.at(i * 2 + 0) != " " || strLineArray.at(i * 2 + 0) != "")
						{
							xVec.push_back(strLineArray.at(i * 2 + 0).toDouble());
							yVec.push_back(strLineArray.at(i * 2 + 1).toDouble());
						}
					}
				}
				ShowLine* showLine = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
				showLine->addSeries(fileNameA + QString::number(i), xVec, yVec);
				xVec.clear();
				yVec.clear();
			}

		}

	}
	else
	{
		qDebug() << "文件打开失败：" << file.errorString();
	}
}

void ManageDevice::showModeling() {
	ui.stackedWidget->setCurrentIndex(1);
	Modeling* modeling = qobject_cast<Modeling*>(ui.stackedWidget->widget(1));
	
}
