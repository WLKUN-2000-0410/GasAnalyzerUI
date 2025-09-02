#include "scantestdlg.h"
#include "showline.h"
#include "GlobalShare.h"
#include "SingletonShared.h"
#include <algorithm>

ScanTestDlg::ScanTestDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initData();
	initView();
	initClick();
}

ScanTestDlg::~ScanTestDlg()
{
}

void ScanTestDlg::initData()
{

}

void ScanTestDlg::initView()
{
	ShowLine* showline = new ShowLine(this);
	//ui.widgetShowline->layout()->addWidget(showline);
	ui.stackedWidget->insertWidget(0, showline);
	ui.stackedWidget->setCurrentIndex(0);
}

void ScanTestDlg::initClick()
{
	connect(this, &ScanTestDlg::addSeriesSignals, this, [this](QString lineName, std::vector<double> xVec, std::vector<double> yVec) {
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		emit showline->addSeriesSignals(lineName,xVec.data(),yVec.data(),xVec.size());
	});
	connect(this, &ScanTestDlg::deleteChoseLine, this, [this]() {
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		QList<int> selectIndexList = showline->getSelected();
		std::reverse(selectIndexList.begin(),selectIndexList.end());
		for (size_t i = 0; i < selectIndexList.size(); i++)
		{
			showline->removeSeries(selectIndexList.at(i));
		}
	});
	
	connect(this, &ScanTestDlg::saveLineByFine, this, [this](QString strFileName) {
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		emit showline->saveSelectDataByFileName(strFileName);
	});
	connect(this, &ScanTestDlg::renameLine, this, [this](int nIndex,QString strNewName) {
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		showline->renameByLine(nIndex,strNewName);
	});
	connect(this, &ScanTestDlg::importByFilePath, this, [this](QString dir) {
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		emit showline->importByFilePathSingle(dir);
	});
	connect(this, &ScanTestDlg::exportByFilePath, this, [this](QString dir) {
		/*QDir funcDir(dir);
		QStringList funcFileList = funcDir.entryList(QDir::Files);*/
		ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
		emit showline->exportByFilePathSingle(dir);
	});
	connect(ui.horizontalSliderLaserPwd, &QSlider::sliderReleased, this, [this]() {
		int nPwd = ui.horizontalSliderLaserPwd->value();
		setLaserPwd(nPwd);
	});
	connect(ui.horizontalSliderLaserPwd, &QSlider::valueChanged, this, [this](int value) {
		ui.labelLaserPwd->setText(QString::number(value));
	});
	connect(ui.pushButtonSingleScan, &QPushButton::clicked, this, &ScanTestDlg::singleScan);
	connect(ui.pushButtonStartScan,&QPushButton::clicked,this,&ScanTestDlg::startScan);
	connect(ui.pushButtonStopScan,&QPushButton::clicked,this,&ScanTestDlg::stopScan);
}

QList<QString> ScanTestDlg::getLineNameList()
{
	ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
	return showline->lineNames;
}

void ScanTestDlg::setLaserPwd(int nLaserPwd)
{
	CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_SET_LASER_PWD, [this]() 
	{
		
	});
	CSingletonShared::GetInstance().setLaserPwd(BussiNumEnum::BUSS_SET_LASER_PWD,nLaserPwd);
}

QList<QJsonObject> ScanTestDlg::getAllDataList()
{
	ShowLine* showline = qobject_cast<ShowLine*>(ui.stackedWidget->widget(0));
	return showline->getAllDataList();
}

void ScanTestDlg::singleScan() 
{
	float fStartWave	= ui.labelLaserPwd->text().toFloat();
	float fEndWave		= ui.lineEditEndWave->text().toFloat();
	float fTime			= ui.lineEditTime->text().toFloat();
	int nCount			= ui.lineEditCount->text().toInt();
	std::string strReadMode = ui.comboBoxReadMode->currentText().toUtf8().constData();
	float fReadFrequency = ui.comboBoxReadFrequency->currentText().toFloat();
	float fRate = ui.comboBoxRate->currentText().toFloat();
	int nGain = ui.comboBoxGain->currentIndex();
	std::string strScanMode = ui.comboBoxScanMode->currentText().toUtf8().constData();
	float fStepTime = ui.lineEditStepTime->text().toFloat();
	CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_SINGLE_SCAN, [this]() {

	});
	CSingletonShared::GetInstance().singleScan(BussiNumEnum::BUSS_SINGLE_SCAN,fStartWave, fEndWave, fTime, nCount
		, strReadMode, fReadFrequency, fRate, nGain, strScanMode, fStepTime);
}
void ScanTestDlg::startScan() 
{
	float fStartWave = ui.labelLaserPwd->text().toFloat();
	float fEndWave = ui.lineEditEndWave->text().toFloat();
	float fTime = ui.lineEditTime->text().toFloat();
	int nCount = ui.lineEditCount->text().toInt();
	std::string strReadMode = ui.comboBoxReadMode->currentText().toUtf8().constData();
	float fReadFrequency = ui.comboBoxReadFrequency->currentText().toFloat();
	float fRate = ui.comboBoxRate->currentText().toFloat();
	int nGain = ui.comboBoxGain->currentIndex();
	std::string strScanMode = ui.comboBoxScanMode->currentText().toUtf8().constData();
	float fStepTime = ui.lineEditStepTime->text().toFloat();
	CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_START_SCAN, [this]() {
	
	});
	CSingletonShared::GetInstance().startScan(BussiNumEnum::BUSS_START_SCAN,fStartWave,fEndWave,fTime,nCount
	,strReadMode,fReadFrequency,fRate,nGain,strScanMode,fStepTime);
}
void ScanTestDlg::stopScan() 
{
	CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_STOP, [this]() {

	});
	CSingletonShared::GetInstance().stop(BussiNumEnum::BUSS_STOP);
}
