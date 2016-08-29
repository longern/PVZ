#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>

class GameLogic : public QObject
{
	Q_OBJECT
public:
	explicit GameLogic(QObject *parent = 0);

	Q_INVOKABLE void onGameStart(QObject *root);
	Q_INVOKABLE void onTimeout(QObject *root);

signals:
	void gameFinished();
};

#endif // GAMELOGIC_H
