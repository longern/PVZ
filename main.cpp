#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	QTranslator translator;
	if (translator.load(QLocale(), QLatin1String("")))
		a.installTranslator(&translator);

	MainWindow w;
	w.show();

	return a.exec();
}
