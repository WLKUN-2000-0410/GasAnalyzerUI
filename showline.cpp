#include "showline.h"


ShowLine::ShowLine(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	initData();
	initView();
	initClick();
}

ShowLine::~ShowLine()
{
}

void ShowLine::saveSelectDataByFileName(QString fileName)
{
	QList<int> selectIndex = getSelected();
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	QStringList fileCont;
	int maxLength = 0;
	for each (int nIndex in selectIndex)
	{
		int nDataCount = customPloat->graph(nIndex)->dataCount();
		maxLength = maxLength > nDataCount ? maxLength : nDataCount;
	}
	QString lineC;
	for (size_t i = 0; i < maxLength; i++)
	{
		QString linedata = "";
		for (size_t j = 0; j < selectIndex.size(); j++)
		{
			int nDataCount = customPloat->graph(selectIndex[j])->dataCount();
			if (i<nDataCount)
			{
				if (j>0)
				{
					linedata.append("\t");
				}
				double key = customPloat->graph(selectIndex[j])->data()->at(i)->key;
				linedata.append(QString::number(key));
				linedata.append("\t");
				double val = customPloat->graph(selectIndex[j])->data()->at(i)->value;
				linedata.append(QString::number(val));

			}
			else
			{
				if (j>0)
				{
					linedata.append("\t");
				}
				linedata.append("");
				linedata.append("\t");
				linedata.append("");
			}
		}
		lineC.append(linedata).append("\n");
	}

	QFile file(fileName);
	if (file.open(QIODevice::WriteOnly)) {  // 文本模式读取
		file.write(lineC.toUtf8());
		file.close();
	}
	else
	{
		qDebug() << "文件打开失败：" << file.errorString();
	}

}

void ShowLine::initData()
{
}

void ShowLine::initView()
{
	ui.widget->setBackground(QColor(31, 31, 31));
	QCPAxisRect* xAxisRect = new QCPAxisRect(ui.widget);
	ui.widget->plotLayout()->addElement(0, 0, xAxisRect);
	QPen axisPens;
	axisPens.setColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widget->setInteractions(QCP::iRangeZoom);
	ui.widget->xAxis->setBasePen(axisPens);
	ui.widget->yAxis->setBasePen(axisPens);
	ui.widget->xAxis->setSubTickPen(axisPens);
	ui.widget->yAxis->setSubTickPen(axisPens);
	ui.widget->xAxis->setTickPen(axisPens);
	ui.widget->yAxis->setTickPen(axisPens);
	ui.widget->xAxis->setLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widget->xAxis->setTickLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widget->yAxis->setLabelColor(QColor(0xFF, 0xFF, 0xFF));
	ui.widget->yAxis->setTickLabelColor(QColor(0xFF, 0xFF, 0xFF));
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	customPloat->legend->setVisible(false);
	// 启用框选放大模式
	customPloat->setSelectionRectMode(QCP::srmZoom);
	// 可选：设置选框样式（虚线边框+半透明填充）
	customPloat->selectionRect()->setPen(QPen(Qt::black, 1, Qt::DashLine));
	customPloat->selectionRect()->setBrush(QBrush(QColor(0, 0, 100, 50)));  
	// 浅蓝色半透明:ml-citation{ref="2,4" data="citationList"}
	connect(customPloat, &QCustomPlot::mouseDoubleClick, [customPloat](){
		customPloat->rescaleAxes();
		customPloat->replot();
	});
	

}

void ShowLine::initClick()
{
	connect(ui.listWidget, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
		qDebug() << u8"点击项内容:" << item->text();
		bool isChecked = item->checkState()==Qt::Checked;
		item->setCheckState(!isChecked?Qt::Checked:Qt::Unchecked);
		isVisbleLine(ui.listWidget->row(item),!isChecked);
	});
	connect(ui.toolButtonReduceX, &QToolButton::clicked, [this]() {
		updateXRange(-1,rangeVal);
	});
	connect(ui.toolButtonAmpX, &QToolButton::clicked, [this]() {
		updateXRange(1, rangeVal);
	});
	connect(ui.toolButtonReduceY, &QToolButton::clicked, [this]() {
		updateYRange(-1, rangeVal);
	});
	connect(ui.toolButtonAmpY, &QToolButton::clicked, [this]() {
		updateYRange(1, rangeVal);
	});
	connect(this, &ShowLine::addSeriesSignals, this, &ShowLine::addSeriesVec);
	connect(this, &ShowLine::saveSelectData, this, &ShowLine::saveSelectDataByFileName);
}

void ShowLine::addSeries(QString lineName, std::vector<double> xList, std::vector<double> yList)
{
	if (lineNames.indexOf(lineName)<0)
	{
		lineNames.append(lineName);
		QJsonObject lineData;
		QJsonArray xjsonArray;
		QJsonArray yjsonArray;
		for (size_t i = 0; i < xList.size(); i++)
		{
			xjsonArray.append(xList.at(i));
			yjsonArray.append(yList.at(i));
		}
		lineData.insert(u8"lineName", lineName);
		lineData.insert(u8"data_x", xjsonArray);
		lineData.insert(u8"data_y", yjsonArray);
		lineObjList.append(lineData);

		QListWidgetItem* item = new QListWidgetItem();
		item->setText(lineName);
		item->setCheckState(Qt::Checked);
		item->setTextColor(QColor(strColor.at(lineNames.size() % strColor.size())));
		//item->setIcon(QIcon("img/yigouxuan.png"));
		item->setSelected(true);
		ui.listWidget->addItem(item);

		QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
		customPloat->legend->setVisible(false);
		customPloat->legend->setFont(QFont("Helvetica", 14));
		QCPGraph * lineGraph = customPloat->addGraph();
		lineGraph->setName(lineName);
		lineGraph->setPen(QPen(QColor(strColor.at(lineNames.size() % strColor.size())), 1));
		lineGraph->setData(xList, yList);
		lineGraph->setLineStyle(QCPGraph::lsLine);
		customPloat->rescaleAxes();
		customPloat->setNotAntialiasedElements(QCP::aeAll);
		customPloat->replot();
	}
	else
	{
		int nIndex = lineNames.indexOf(lineName);
		QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
		customPloat->graph(nIndex)->setData(xList, yList);
		customPloat->rescaleAxes();
		customPloat->setNotAntialiasedElements(QCP::aeAll);
		customPloat->replot();
		QJsonObject lineData = lineObjList.at(nIndex);
		QJsonArray xjsonArray;
		QJsonArray yjsonArray;
		for (size_t i = 0; i < xList.size(); i++)
		{
			xjsonArray.append(xList.at(i));
			yjsonArray.append(yList.at(i));
		}

	}
	
}
void ShowLine::addSeriesVec(QString lineName, double * xArray, double * yArray, int length)
{
	std::vector<double> xList;
	std::vector<double> yList;
	for (size_t i = 0; i < length; i++)
	{
		xList.push_back(xArray[i]);
		yList.push_back(yArray[i]);

	}
	addSeries(lineName,xList,yList);
}
void ShowLine::removeSeries(QString lineName) 
{
	int nIndex = lineNames.indexOf(lineName);
	lineNames.removeAt(nIndex);
	removeSeries(nIndex);
	lineObjList.removeAt(nIndex);
}
void ShowLine::removeSeries(int nIndex) 
{
	QListWidgetItem* item = ui.listWidget->takeItem(nIndex);
	if (item) {
		// 先解除关联的widget
		QWidget* widget = ui.listWidget->itemWidget(item);
		if (widget) {
			ui.listWidget->removeItemWidget(item);
			widget->deleteLater();
		}
		// 再删除item
		delete ui.listWidget->takeItem(ui.listWidget->row(item));
	}

	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	customPloat->removeGraph(nIndex);
	customPloat->replot();
	//lineNames.removeAt(nIndex);
}

void ShowLine::removeAllSeries()
{
	lineNames.clear();
	lineObjList.clear();
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	customPloat->clearGraphs();
	customPloat->replot();

	ui.listWidget->clear();
}

void ShowLine::updateXRange(int nReduce, double range)
{
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	QCPRange nRange = customPloat->xAxis->range();
	double xMinRange = nRange.lower + range*nReduce / 2 * (nRange.upper- nRange.lower);
	double xMaxRange = nRange.upper + range*nReduce / 2 * (nRange.lower- nRange.upper);
	customPloat->xAxis->setRange(xMinRange,xMaxRange);
	customPloat->replot();
}

void ShowLine::updateYRange(int nReduce, double range)
{
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	QCPRange nRange = customPloat->yAxis->range();
	double yMinRange = nRange.lower + range*nReduce / 2 * (nRange.upper - nRange.lower);
	double yMaxRange = nRange.upper + range*nReduce / 2 * (nRange.lower - nRange.upper);
	customPloat->yAxis->setRange(yMinRange, yMaxRange);
	customPloat->replot();
}

QString ShowLine::getSeriesName()
{
	double dVal = nIndex / 1000.0;
	QString strVal = QString::number(dVal);
	nIndex++;
	return u8"S" + strVal.mid(2);
}

void ShowLine::isVisbleLine(int nIndex,bool isHidden)
{
	QCustomPlot* customPloat = qobject_cast<QCustomPlot*>(ui.widget);
	customPloat->graph(nIndex)->setVisible(isHidden);
	customPloat->replot();
}

QList<int> ShowLine::getSelected()
{
	QList<int> selectedIndex;
	for (size_t i = 0; i < lineNames.size(); i++)
	{
		QListWidgetItem* item = ui.listWidget->item(i);

		if (item->checkState()==Qt::Checked)
		{
			selectedIndex.push_back(i);
		}
	}
	return selectedIndex;
}

QList<int> ShowLine::getUnSelected()
{
	QList<int> unSelectedIndex;
	for (size_t i = 0; i < lineNames.size(); i++)
	{
		QListWidgetItem* item = ui.listWidget->item(i);
		if (item->checkState() == Qt::Unchecked)
		{
			unSelectedIndex.push_back(i);
		}
	}
	return unSelectedIndex;
}

QList<QJsonObject> ShowLine::getSelectedByObj()
{
	QList<QJsonObject> selectedIndex;
	for (size_t i = 0; i < lineNames.size(); i++)
	{
		QListWidgetItem* item = ui.listWidget->item(i);

		if (item->checkState() == Qt::Checked)
		{
			selectedIndex.push_back(lineObjList.at(i));
		}
	}
	return selectedIndex;
}

QList<QJsonObject> ShowLine::getUnSelectedByObj()
{
	QList<QJsonObject> unSelectedIndex;
	for (size_t i = 0; i < lineNames.size(); i++)
	{
		QListWidgetItem* item = ui.listWidget->item(i);
		if (item->checkState() == Qt::Unchecked)
		{
			unSelectedIndex.push_back(lineObjList.at(i));
		}
	}
	return unSelectedIndex;
}

QJsonObject ShowLine::getDataByLineName(QString lineName)
{
	int nIndex = lineNames.indexOf(lineName);
	return lineObjList.at(nIndex);
}

QList<QJsonObject> ShowLine::getAllDataList()
{
	return lineObjList;
}

