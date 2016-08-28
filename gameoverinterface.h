#ifndef GAMEOVERINTERFACE_H
#define GAMEOVERINTERFACE_H

#include <QWidget>
#include "mainwindow.h"

class GameOverInterface : public QWidget
{
	Q_OBJECT
public:
	explicit GameOverInterface(QWidget *parent = 0);

signals:
	void stateSet(MainWindow::GameState);

public slots:
};

#endif // GAMEOVERINTERFACE_H
