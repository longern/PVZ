#include "gamelogic.h"
#include "plants/plants.h"
#include "zombies/zombies.h"
#include <QVariant>
#include <QElapsedTimer>

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}

void GameLogic::onGameStart(QObject *root)
{
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	root->setProperty("gameStartTime", QVariant(elapsedTimer.msecsSinceReference()));
	root->setProperty("currentTime", QVariant(qint64(0)));
}

void GameLogic::onTimeout(QObject *root)
{
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	qint64 newCurrentTime = elapsedTimer.msecsSinceReference() - root->property("gameStartTime").toLongLong();
	qint64 oldCurrentTime = root->property("currentTime").toLongLong();
	if(oldCurrentTime < 1000 && newCurrentTime >= 1000)
	{
		QPointer<Zombie> newZombie = dynamic_cast<Zombie *>(GetZombieClassByID(1)->newInstance());
		newZombie->setPos(QPointF(8, qrand() % 5));

		QList<QVariant> zombiesData(root->property("zombies").toList());
		zombiesData.append(QVariant::fromValue(newZombie));
		root->setProperty("zombies", zombiesData);
	}
	root->setProperty("currentTime", newCurrentTime);
}
