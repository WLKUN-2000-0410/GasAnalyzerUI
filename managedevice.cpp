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
	int i = CustomMessageBox::showCustomm(3, QString::fromLocal8Bit("ȷ��Ҫ�˳���?")
		, QString::fromLocal8Bit("��"), QString::fromLocal8Bit("��"));
	qDebug() << i;

	if (i == CustomMessageBox::Result::Confirm) {

	}
	else {
		event->ignore();  // ��ֹ�ر�
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
	actionFileOpen = menuFile->addAction(QString::fromLocal8Bit("��"));
	actionFileSave = menuFile->addAction(QString::fromLocal8Bit("����"));
	actionFileImport = menuFile->addAction(QString::fromLocal8Bit("����"));
	ui.toolButton_file->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_file->setMenu(menuFile);

	QMenu* menujiao = new QMenu(this);
	actionJiao1 = menujiao->addAction(QString::fromLocal8Bit("У��"));
	actionJiao2 = menujiao->addAction(QString::fromLocal8Bit("�ο�̽����"));
	actionJiao3 = menujiao->addAction(QString::fromLocal8Bit("У׼"));
	actionJiao4 = menujiao->addAction(QString::fromLocal8Bit("��·���"));
	ui.toolButton_jiaozheng->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_jiaozheng->setMenu(menujiao);

	QMenu* menuScan = new QMenu(this);
	actionScan1 = menuScan->addAction(QString::fromLocal8Bit("̽����"));
	actionScan2 = menuScan->addAction(QString::fromLocal8Bit("������"));
	actionScan3 = menuScan->addAction(QString::fromLocal8Bit("�ɼ�"));
	ui.toolButton_scan->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_scan->setMenu(menuScan);

	QMenu* menuDevice = new QMenu(this);
	actionDevice1 = menuDevice->addAction(QString::fromLocal8Bit("̽����"));
	actionDevice2 = menuDevice->addAction(QString::fromLocal8Bit("������"));
	actionDevice3 = menuDevice->addAction(QString::fromLocal8Bit("������"));
	actionDevice4 = menuDevice->addAction(QString::fromLocal8Bit("�¶ȼ�"));
	actionDevice5 = menuDevice->addAction(QString::fromLocal8Bit("ѹ����"));
	actionDevice6 = menuDevice->addAction(QString::fromLocal8Bit("������"));
	actionDevice7 = menuDevice->addAction(QString::fromLocal8Bit("��ձ�"));
	actionDevice8 = menuDevice->addAction(QString::fromLocal8Bit("�ʵ�"));
	actionDevice9 = menuDevice->addAction(QString::fromLocal8Bit("������"));
	ui.toolButton_device->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_device->setMenu(menuDevice);

	QMenu* menuDB = new QMenu(this);
	actionDB1 = menuDB->addAction(QString::fromLocal8Bit("��ģ"));
	actionDB2 = menuDB->addAction(QString::fromLocal8Bit("�鿴��ģ"));
	ui.toolButton_db->setPopupMode(QToolButton::InstantPopup);
	ui.toolButton_db->setMenu(menuDB);

	QMenu* menuMath = new QMenu(this);
	actionMath1 = menuMath->addAction(QString::fromLocal8Bit("�������"));
	actionMath2 = menuMath->addAction(QString::fromLocal8Bit("���߱��"));
	actionMath3 = menuMath->addAction(QString::fromLocal8Bit("����ƽ��"));
	actionMath4 = menuMath->addAction(QString::fromLocal8Bit("���������߿�"));
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
		QString::fromLocal8Bit("ѡ���ļ�"),
		"",
		QString::fromLocal8Bit("�ı��ļ� (*.txt);")
	);
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // �ı�ģʽ��ȡ
															 // �����ļ�
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
		qDebug() << "�ļ���ʧ�ܣ�" << file.errorString();
	}
}

void ManageDevice::showModeling() {
	ui.stackedWidget->setCurrentIndex(1);
	Modeling* modeling = qobject_cast<Modeling*>(ui.stackedWidget->widget(1));
	
}
