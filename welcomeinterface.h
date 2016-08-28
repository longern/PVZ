#ifndef WELCOMEINTEFACE_H
#define WELCOMEINTEFACE_H

#include <QWidget>
#include <QPainter>
#include "mainwindow.h"

namespace Ui {
class WelcomeForm;
}

class WelcomeInterface : public QWidget
{
	Q_OBJECT
public:
	explicit WelcomeInterface(QWidget *parent = 0);

signals:
	void stateSet(MainWindow::GameState);

public slots:

private:
	Ui::WelcomeForm *ui;

private slots:
	void on_buttonBeginAdventure_clicked();
	void on_buttonChangeUser_clicked();
};

#endif // WELCOMEINTEFACE_H
