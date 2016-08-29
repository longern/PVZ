#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>

class GameLogic : public QObject
{
	Q_OBJECT
public:
	explicit GameLogic(QObject *parent = 0);

signals:

public slots:
};

#endif // GAMELOGIC_H
