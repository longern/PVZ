#include "laststandinterface.h"

LastStandInterface::LastStandInterface(QWidget *parent) : QWidget(parent)
{
	playInterface = new PlayingInterface(this);
	playInterface->gameStatus()->setProperty("mode", "lastStand");
	connect(playInterface, SIGNAL(stateSet(MainWindow::GameState)), this, SIGNAL(stateSet(MainWindow::GameState)));
	playInterface->show();
}
