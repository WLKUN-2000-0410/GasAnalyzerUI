#include "smoothdialog.h"

SmoothDialog::SmoothDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlag(Qt::FramelessWindowHint);

	initData();
	initView();
	initClick();
}

SmoothDialog::~SmoothDialog()
{
}

void SmoothDialog::initData()
{
}

void SmoothDialog::initView()
{

}

void SmoothDialog::initClick()
{
	connect(ui.pushButton_Close, &QPushButton::clicked, this, [this]() {
		close();
	});
	connect(ui.pushButton, &QPushButton::clicked, this, [this]() {
		ui.comboBoxChooseLine->currentText();
		ui.comboBoxChoosePram->currentText().toInt();


	});
	connect(ui.comboBoxChooseLine, QOverload<int>::of(&QComboBox::activated), [this](int nIndex) {
		int i = nIndex;
	});
}

void SmoothDialog::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		if (event->pos().ry()<30)
		{
			m_dragging = true;
			m_dragPosition = event->globalPos() - frameGeometry().topLeft();
		}
	}
}

void SmoothDialog::mouseMoveEvent(QMouseEvent * event)
{
	if (m_dragging) {
		move(event->globalPos() - m_dragPosition);
	}
}

void SmoothDialog::mouseReleaseEvent(QMouseEvent * event)
{
	m_dragging = false;
}
