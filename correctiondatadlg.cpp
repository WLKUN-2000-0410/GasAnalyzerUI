#include "correctiondatadlg.h"
#include <QtCore/qobjectdefs.h>
#include "listitemwidget.h"
#include "GlobalShare.h"
#include "SingletonShared.h"

CorrectionDataDlg::CorrectionDataDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlag(Qt::FramelessWindowHint);

	initData();
	initView();
	initClick();
}

CorrectionDataDlg::~CorrectionDataDlg()
{

}

void CorrectionDataDlg::initData()
{
	readInit();
}

void CorrectionDataDlg::initView()
{
	ui.widgetChart->setBackground(QColor(31, 31, 31));
	QCPAxisRect* xAxisRect = new QCPAxisRect(ui.widgetChart);
	ui.widgetChart->plotLayout()->addElement(0, 0, xAxisRect);
	QPen axisPens;
	axisPens.setColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widgetChart->setInteractions(QCP::iRangeZoom);
	ui.widgetChart->xAxis->setBasePen(axisPens);
	ui.widgetChart->yAxis->setBasePen(axisPens);
	ui.widgetChart->xAxis->setSubTickPen(axisPens);
	ui.widgetChart->yAxis->setSubTickPen(axisPens);
	ui.widgetChart->xAxis->setTickPen(axisPens);
	ui.widgetChart->yAxis->setTickPen(axisPens);
	ui.widgetChart->xAxis->setLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widgetChart->xAxis->setTickLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widgetChart->yAxis->setLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widgetChart->yAxis->setTickLabelColor(QColor(0xFF, 0xFF, 0xFF));
	
	ui.widgetChart->legend->setVisible(false);
	// 启用框选放大模式
	ui.widgetChart->setSelectionRectMode(QCP::srmZoom);
	// 可选：设置选框样式（虚线边框+半透明填充）
	ui.widgetChart->selectionRect()->setPen(QPen(Qt::black, 1, Qt::DashLine));
	ui.widgetChart->selectionRect()->setBrush(QBrush(QColor(0, 0, 100, 50)));
	// 浅蓝色半透明:ml-citation{ref="2,4" data="citationList"}
	connect(ui.widgetChart, &QCustomPlot::mouseDoubleClick, [this]() {
		ui.widgetChart->rescaleAxes();
		ui.widgetChart->replot();
	});

}

void CorrectionDataDlg::initClick()
{
	connect(ui.pushButton_Close, &QPushButton::clicked, this, &CorrectionDataDlg::close);
	connect(this,&CorrectionDataDlg::updateListByIndexSingle,this,&CorrectionDataDlg::updateListByIndex);
	connect(ui.comboBoxChooseLine, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int i) {
		emit updateListByIndexSingle(i);
	});
	connect(this, &CorrectionDataDlg::initTableViewSingle, this, &CorrectionDataDlg::initTableView);
	connect(ui.pushButtonCorrection, &QPushButton::clicked, this, &CorrectionDataDlg::correction);
	connect(ui.pushButtonExportNum,&QPushButton::clicked,this,&CorrectionDataDlg::exportNumIni);
	connect(ui.pushButtonAutoFill, &QPushButton::clicked, this, &CorrectionDataDlg::autoFill);
	connect(ui.pushButtonLaserCorrection, &QPushButton::clicked, this, &CorrectionDataDlg::laserCorrection);
	emit initTableViewSingle();
}

void CorrectionDataDlg::setLineObjList(QList<QJsonObject> lineList)
{
	lineObjList = lineList;
	for (size_t i = 0; i < lineObjList.size(); i++)
	{
		ui.comboBoxChooseLine->addItem(lineObjList.at(i).value("lineName").toString());
	}
	emit updateListByIndexSingle(0);
}

void CorrectionDataDlg::readInit()
{
	QSettings* m_settings = new QSettings("ini/wave.ini", QSettings::IniFormat);
	m_settings->setIniCodec("UTF-8");
	/****************************************************************/
	m_settings->beginGroup(QString::fromLocal8Bit("CCD"));
	QString ccd_model = m_settings->value(QString::fromLocal8Bit("Model"), "").toString();
	QString ccd_number = m_settings->value(QString::fromLocal8Bit("Number"), "").toString();
	int ccd_ominiType = m_settings->value(QString::fromLocal8Bit("OminiType"), 0).toInt();
	waveObj.insert(u8"model",ccd_model);
	waveObj.insert(u8"number",ccd_number);
	waveObj.insert(u8"ominitype",ccd_ominiType);
	m_settings->endGroup();

	m_settings->beginGroup(QString::fromLocal8Bit("CS"));
	QStringList allkey = m_settings->allKeys();
	QJsonArray strjsoArray;
	for (size_t i = 0; i < allkey.size(); i++)
	{
		float fs = m_settings->value(QString::fromLocal8Bit("a%1").arg(i)).toFloat();
		strjsoArray.append(QString::number(fs, 'f', 1));
	}
	waveObj.insert(u8"CS", strjsoArray);
	m_settings->endGroup();

	m_settings->beginGroup(QString::fromLocal8Bit("sz"));
	float laser = m_settings->value(u8"Laser").toFloat();
	waveObj.insert(u8"laser",laser);
	m_settings->endGroup();

	m_settings->beginGroup(QString::fromLocal8Bit("bxjzxs"));
	allkey = m_settings->allKeys();
	QJsonArray strjsonArray;
	for (size_t i = 0; i < allkey.size(); i++)
	{
		float fs = m_settings->value(QString::fromLocal8Bit("a%1").arg(i)).toFloat();
		strjsonArray.append(QString::number(fs, 'f', 1));
	}
	waveObj.insert(u8"bxjzxs",strjsonArray);
	m_settings->endGroup();

	
}

void CorrectionDataDlg::initTableView()
{
	QJsonArray bar = waveObj.value(u8"bxjzxs").toArray();
	QJsonArray car = waveObj.value(u8"CS").toArray();
	for (size_t i = 0; i < bar.size(); i++)
	{
		QListWidgetItem *item = new QListWidgetItem();
		// 创建自定义Widget
		ListItemWidget *widget = new ListItemWidget();
		widget->setFixedWidth(160);
		
		QString strWave = bar.at(i).toString();
		QString strPix = car.at(i).toString();
		
		if (strWave==u8"0.0"&&strPix==u8"0.0")
		{
			break;
		}

		widget->setWave(strWave);
		widget->setPix(strPix);
		// 设置列表项大小
		item->setSizeHint(QSize(80,30));
		
		//item->setSizeHint(widget->sizeHint());
		// 添加到列表
		ui.listWidget->addItem(item);
		ui.listWidget->setItemWidget(item, widget);

		connect(widget, &ListItemWidget::updatePix, this, &CorrectionDataDlg::updatePix);
	}
}

void CorrectionDataDlg::updatePix(int nIndex, QString strWave)
{
	QJsonArray car = waveObj.value(u8"CS").toArray();
	car.insert(nIndex,strWave);
	waveObj.insert(u8"CS",strWave);
}

void CorrectionDataDlg::correction()
{
	int nCount = ui.listWidget->count();
	std::vector<double> waveVec;
	std::vector<double> pixVec;
	for (size_t i = 0; i < nCount; i++)
	{
		ListItemWidget* widget = qobject_cast<ListItemWidget*> (ui.listWidget->item(i)->listWidget());
		waveVec.push_back(widget->getWave().toDouble());
		pixVec.push_back(widget->getPix().toDouble());
	}
	CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_CORRECTION_DATA, [this]() {
		
	});
	CSingletonShared::GetInstance().correctionData(BussiNumEnum::BUSS_CORRECTION_DATA,waveVec,pixVec);
}

void CorrectionDataDlg::exportNumIni()
{
	QString fileName = QFileDialog::getSaveFileName(
		this,
		QString::fromLocal8Bit("选择文件"),
		"",
		QString::fromLocal8Bit("配置文件 (*.ini);")
	);

	QSettings* m_settings = new QSettings("ini/wave.ini", QSettings::IniFormat);
	m_settings->setIniCodec("UTF-8");

	int nCount = ui.listWidget->count();
	m_settings->beginGroup(QString::fromLocal8Bit("CS"));
	for (size_t i = 0; i < nCount; i++)
	{
		ListItemWidget* widget = qobject_cast<ListItemWidget*> (ui.listWidget->item(i)->listWidget());
		m_settings->setValue(QString::fromLocal8Bit("a%1").arg(i), widget->getPix());
	}
	m_settings->endGroup();
	m_settings->beginGroup(QString::fromLocal8Bit("bxjzxs"));
	for (size_t i = 0; i < nCount; i++)
	{
		ListItemWidget* widget = qobject_cast<ListItemWidget*> (ui.listWidget->item(i)->listWidget());
		m_settings->setValue(QString::fromLocal8Bit("a%1").arg(i), widget->getWave());
	}
	m_settings->endGroup();
	
	copyFileToPath("ini/wave.ini",fileName);

}

void CorrectionDataDlg::autoFill()
{

}

void CorrectionDataDlg::laserCorrection()
{

}

void CorrectionDataDlg::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().ry()<30)
		{
			m_dragging = true;
			m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
	}
}

void CorrectionDataDlg::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragging) {
		move(event->globalPos() - m_dragPosition);
	}
}

void CorrectionDataDlg::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
}

void CorrectionDataDlg::updateListByIndex(int nIndex) 
{
	QJsonObject jsonObj = lineObjList.at(nIndex);
	ui.comboBoxChooseLine->setCurrentIndex(nIndex);
	std::vector<double> xList;
	std::vector<double> yList;
	QJsonArray data_x = jsonObj.value(u8"data_x").toArray();
	QJsonArray data_y = jsonObj.value(u8"data_y").toArray();
	for (int i = 0; i < data_x.size(); i++)
	{
		xList.push_back(i);
		yList.push_back(data_y.at(i).toDouble());
	}

	int nCount = ui.widgetChart->graphCount();
	if (nCount==0)
	{
		ui.widgetChart->legend->setVisible(false);
		ui.widgetChart->legend->setFont(QFont("Helvetica", 14));
		QCPGraph * lineGraph = ui.widgetChart->addGraph();
		lineGraph->setName(jsonObj.value("lineName").toString());
		lineGraph->setPen(QPen(QColor(strColor.at(nIndex % strColor.size())), 1));
		lineGraph->setData(xList, yList);
		lineGraph->setLineStyle(QCPGraph::lsLine);
		ui.widgetChart->rescaleAxes();
		ui.widgetChart->setNotAntialiasedElements(QCP::aeAll);
		ui.widgetChart->replot();
	}
	else 
	{
		ui.widgetChart->graph(0)->setName(jsonObj.value("lineName").toString());
		ui.widgetChart->graph(0)->setData(xList, yList);
		ui.widgetChart->rescaleAxes();
		ui.widgetChart->setNotAntialiasedElements(QCP::aeAll);
		ui.widgetChart->replot();
	}
}

bool CorrectionDataDlg::copyFileToPath(const QString & sourcePath, const QString & destPath, bool overwrite)
{
	// 检查源文件是否存在
	if (!QFile::exists(sourcePath)) {
		qWarning() << "Source file does not exist:" << sourcePath;
		return false;
	}
	
	// 检查目标目录是否存在
	QDir destDir(QFileInfo(destPath).absolutePath());
	if (!destDir.exists()) {
		if (!destDir.mkpath(".")) {
			qWarning() << "Failed to create destination directory:" << destDir.path();
			return false;
		}
	}
	
	// 处理目标文件已存在的情况
	if (QFile::exists(destPath)) {
		if (!overwrite) {
			qWarning() << "Destination file already exists and overwrite is disabled:" << destPath;
			return false;
		}
		if (!QFile::remove(destPath)) {
			qWarning() << "Failed to remove existing destination file:" << destPath;
			return false;
		}
	}
	
	// 执行文件复制
	if (!QFile::copy(sourcePath, destPath)) {
		qWarning() << "Failed to copy file from" << sourcePath << "to" << destPath;
		return false;
	}
	
	qDebug() << "File copied successfully from" << sourcePath << "to" << destPath;
	return true;
}