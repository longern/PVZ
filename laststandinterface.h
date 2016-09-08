#ifndef LASTSTANDINTERFACE_H
#define LASTSTANDINTERFACE_H

#include "playinginterface.h"

class LastStandInterface : public QWidget
{
	Q_OBJECT
public:
	explicit LastStandInterface(QWidget *parent = 0);

signals:
	void stateSet(MainWindow::GameState);

public slots:

private:
	PlayingInterface *playInterface;
};

#endif // LASTSTANDINTERFACE_H
