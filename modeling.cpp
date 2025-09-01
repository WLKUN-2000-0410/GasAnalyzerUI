#include "modeling.h"
#include <QDebug>
#include "SingletonShared.h"
#include "GlobalShare.h"
#include <QDir>
#include "smoothdialog.h"

Modeling::Modeling(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initData();
}

Modeling::~Modeling() 
{
}

void Modeling::initData()
{

	

	initView();
}


void Modeling::initView()
{
	folderJsonArray = readCheckJsonByFile();
	

	for (size_t i = 0; i < folderJsonArray.size(); i++)
	{
		QJsonObject fJsonObj = folderJsonArray.at(i).toObject();
		ui.comboBoxModeList->addItem(fJsonObj.value("lineName").toString());
		QString filePath = fJsonObj.value("checked").toBool() 
			? u8":/UiDemo/img/32.png" : u8":/UiDemo/img/33.png";
		ui.comboBoxTestSample->addItem(QIcon(filePath), fJsonObj.value("lineName").toString());
	}
	ui.comboBoxTestSample->setIconSize(QSize(18,18));

	ui.lineEditMol->setDisabled(true);
	for (size_t i = 10000; i < 1000001; i+=10000)
	{
		ui.comboBoxCentration->addItem(QString::number(i));
	}
	initClick();
}

void Modeling::initClick()
{
	connect(ui.pushButtonModeling, &QPushButton::clicked, this, [this]() {
		bool isSubBaseline = ui.checkBoxSubBaseline->isChecked();
		bool isSmooth = ui.checkBoxSmooth->isChecked();
		float fPeakPosition = ui.comboBoxPeakPosition->currentText().toDouble();
		std::string strTestName = ui.lineEditTestName->text().toUtf8().constData();
		float fCentration = ui.comboBoxCentration->currentText().toDouble();
		float fMol = ui.lineEditMol->text().toDouble();
		float fTotalPa = ui.lineEditTotalPa->text().toDouble();
		float fIteamPa = ui.lineEditItemPa->text().toDouble();
		std::string strModel = ui.comboBoxModeList->currentText().toUtf8().constData();
		std::string strSample = ui.comboBoxTestSample->currentText().toUtf8().constData();

		CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_MODELING, [this]() {
			
		});
		CSingletonShared::GetInstance().scanModeling(BussiNumEnum::BUSS_MODELING, nTestMode, isSubBaseline, isSmooth, fPeakPosition
			, strTestName, fCentration, fMol, fTotalPa, fIteamPa);
	});
	connect(this, &Modeling::showSmoothDialogSignale,this,&Modeling::showSmoothDialog);
	connect(ui.pushButtonVerify, &QPushButton::clicked, this, [this]() {
		bool isSubBaseline = ui.checkBoxSubBaseline->isChecked();
		bool isSmooth = ui.checkBoxSmooth->isChecked();
		float fPeakPosition = ui.comboBoxPeakPosition->currentText().toDouble();
		std::string strTestName = ui.lineEditTestName->text().toUtf8().constData();
		float fCentration = ui.comboBoxCentration->currentText().toDouble();
		float fMol = ui.lineEditMol->text().toDouble();
		float fTotalPa = ui.lineEditTotalPa->text().toDouble();
		float fIteamPa = ui.lineEditItemPa->text().toDouble();
		std::string strModel = ui.comboBoxModeList->currentText().toUtf8().constData();
		std::string strSample = ui.comboBoxTestSample->currentText().toUtf8().constData();


		CSingletonShared::GetInstance().setFuntionCallbackMap(BussiNumEnum::BUSS_VERIFY, [this]() {
			int i = 0;
		});
		CSingletonShared::GetInstance().scanVerify(BussiNumEnum::BUSS_VERIFY, nTestMode,isSubBaseline,isSmooth,fPeakPosition
			,strTestName,fCentration,fMol,fTotalPa,fIteamPa);
	
	});
	connect(ui.radioButtonPressure, &QPushButton::clicked, this, [this]() {
		nTestMode = 0;
	});
	connect(ui.radioButtonconCentration, &QPushButton::clicked, this, [this]() {
		nTestMode = 1;
	});
	connect(ui.checkBoxSmooth, &QCheckBox::clicked, this, [this]() {
		bool isChecked = ui.checkBoxSmooth->isChecked();
		if (isChecked)
		{
			
			emit showSmoothDialogSignale();
		}
	});
	connect(ui.comboBoxPeakPosition, &QComboBox::currentTextChanged, [this] (QString test){
		qDebug() << test;
	});
	connect(ui.comboBoxCentration, &QComboBox::currentTextChanged, [this](QString strText) {
		ui.lineEditMol->setText(strText.mid(0,strText.size()-4));
	});
	connect(ui.comboBoxTestSample, QOverload<int>::of(&QComboBox::activated), [this](int nIndex) {
		QJsonObject jsonObj = folderJsonArray.at(nIndex).toObject();
		bool checked = jsonObj.value("checked").toBool();
		jsonObj["checked"] = !checked;
		folderJsonArray[nIndex] = jsonObj;
		writeCheckMode(folderJsonArray);

		emit setComboxIconSignals(nIndex, !checked
			? u8":/UiDemo/img/32.png" : u8":/UiDemo/img/33.png");
	});
	connect(this, &Modeling::setComboxIconSignals, this, &Modeling::setComboxIcon);
}

void Modeling::setComboxIcon(int nIndex, QString iconPath)
{
	ui.comboBoxTestSample->setItemIcon(nIndex,QIcon(iconPath));
}

void Modeling::setLineNameList(QList<QString> nameList)
{
	lineNameList.clear();
	for (size_t i = 0; i < nameList.size(); i++)
	{
		lineNameList.append(nameList.at(i));
		QListWidgetItem* item = new QListWidgetItem();
		item->setText(nameList.at(i));
		item->setCheckState(Qt::Checked);
		item->setTextColor(QColor(strColor.at(nameList.size() % strColor.size())));
		//item->setIcon(QIcon("img/yigouxuan.png"));
		item->setSelected(true);
		ui.listWidget->addItem(item);
	}
	//ui.comboBoxTestSample->addItems(lineNameList);
	
}

void Modeling::writeCheckMode(QJsonArray json)
{
	QJsonDocument doc(json);

	// 写入文件
	QFile file("modeList/mainCheckJson.json");
	if (file.open(QIODevice::WriteOnly)) {
		file.write(doc.toJson());
		file.close();
		qDebug() << "JSON array saved to file";
	}
	else {
		qWarning() << "Failed to open file for writing";
	}
}

QJsonArray Modeling::readCheckJsonByFile()
{
	QJsonArray jsonA;
	QFile inputFile("modeList/mainCheckJson.json");
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "无法打开输入文件";
		return jsonA;
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(inputFile.readAll(), &error);
	if (error.error != QJsonParseError::NoError) {
		qDebug() << "JSON解析错误:" << error.errorString();
		return jsonA;
	}
	
	inputFile.close();
	return doc.array();
}

void Modeling::showSmoothDialog()
{
	SmoothDialog* smooth = new SmoothDialog(this);
	smooth->show();
}
