#include "UiDemo.h"
#include "managedevice.h"
#include <QtWidgets/QApplication>
#include <qfile.h>
#include <qfontdatabase.h>
#include <QIcon>
#include <QMovie>
#include <QTimer>
#include <QDebug>

void loadFontWeights() {
	const QStringList weights = { "Light", "Regular", "Bold" ,"Demibold", "ExtraLight", "Heavy", "Medium", "Normal", "Semibold", "Thin" };
	for (const auto &w : weights) {
		QString path = QString(":/fonts/MiSans-%1.ttf").arg(w);
		if (QFontDatabase::addApplicationFont(path) == -1) {
			qWarning() << "Font load failed:" << path;
		}
	}
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	/*ManageDevice manageDevice;
	manageDevice.show();*/
	QFile qssFile("styles/solarSparkApp.css");
	if (qssFile.open(QIODevice::ReadOnly)) {
		QString styleSheet = QString::fromUtf8(qssFile.readAll());
		a.setStyleSheet(styleSheet);
	}
	loadFontWeights();

	// 使用Bold字重
	QFont font("MiSans");
	font.setPixelSize(12);
	//	font.setWeight(QFont::Regular);
	a.setFont(font);


	ManageDevice w;
	w.setWindowIcon(QIcon(":/UiDemo/img/favicon64_64.ico")); // 全局图标
	w.show();

    return a.exec();
}
