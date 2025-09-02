#include "renamebylinedlg.h"


RenameByLineDlg::RenameByLineDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	scanTestDlg = qobject_cast<ScanTestDlg*>(parent);
	initData();
	initView();
	initClick();
}

RenameByLineDlg::~RenameByLineDlg()
{
}

void RenameByLineDlg::initData()
{
}

void RenameByLineDlg::initView()
{
}

void RenameByLineDlg::initClick()
{
	connect(ui.comboBoxChooseLine, &QComboBox::currentTextChanged, this, [this](QString linename) {
		ui.lineEditNewLineName->setText(linename);
	});
	connect(ui.pushButtonUpdateName, &QPushButton::clicked, this, [this]() {
		int nIndex = ui.comboBoxChooseLine->currentIndex();
		QString strNewLineName = ui.lineEditNewLineName->text();
		ui.comboBoxChooseLine->setItemText(nIndex,strNewLineName);
		emit scanTestDlg->renameLine(nIndex,strNewLineName);
	});
	connect(ui.pushButton_Close, &QPushButton::clicked, this, &RenameByLineDlg::close);
}

void RenameByLineDlg::setLineNameList(QList<QString> nameList)
{
	ui.comboBoxChooseLine->addItems(nameList);
}

void RenameByLineDlg::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().ry()<30)
		{
			m_dragging = true;
			m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
	}
}

void RenameByLineDlg::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragging) {
		move(event->globalPos() - m_dragPosition);
	}
}

void RenameByLineDlg::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
}
