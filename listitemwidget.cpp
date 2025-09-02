#include "listitemwidget.h"

ListItemWidget::ListItemWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.lineEditWave, &QLineEdit::editingFinished, this, [this]() {

		emit updatePix(nIndex,ui.lineEditWave->text());
	});
}

ListItemWidget::~ListItemWidget()
{
}

void ListItemWidget::setWave(QString strWave)
{
	ui.lineEditWave->setText(strWave);
}

void ListItemWidget::setPix(QString strPwd)
{
	ui.lineEditPix->setText(strPwd);
}

void ListItemWidget::setIndex(int index)
{
	nIndex = index;
}

QString ListItemWidget::getWave()
{
	return ui.lineEditWave->text();
}

QString ListItemWidget::getPix()
{
	return ui.lineEditPix->text();
}

int ListItemWidget::getIndex()
{
	return nIndex;
}
