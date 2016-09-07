#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QPair>
#include <functional>

class Zombie;

class GameLogic : public QObject
{
	Q_OBJECT
public:
	explicit GameLogic(QObject *parent = 0);

	Q_INVOKABLE void startGame(QObject *root);
	Q_INVOKABLE void onTimeout(QObject *root);
	Q_INVOKABLE void createPlant(QObject *root, int plantType, double x, double y);
	Q_INVOKABLE void createZombie(QObject *root, int zombieType, double x, double y);
	Q_INVOKABLE void createSunshine(QObject *root, double x);
	Q_INVOKABLE void collectSunshine(QObject *root, int index);
	Q_INVOKABLE void addTimeFlag(qint64 timePoint, std::function<void(QObject *)> func);

signals:
	void zombieCreated(Zombie *);
	void sunshineCreated();
	void sunshineCollected(int);
	void gameFinished();

private:
	QList<QPair<qint64, std::function<void (QObject *)>>> timeFlags;
};

#endif // GAMELOGIC_H
