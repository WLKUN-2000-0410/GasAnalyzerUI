#include "scantestdlg.h"
#include "showline.h"

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
	ui.widgetShowline->layout()->addWidget(showline);
}

void ScanTestDlg::initClick()
{
	
}
