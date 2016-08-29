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
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	QSize mapSize = root->property("mapSize").toSize();

	for (const QVariant &y : root->property("zombies").toList())
	{
		Zombie *zombie = (Zombie *)(y.value<QPointer<Zombie>>());
		if(zombie->hp() > 0 && zombie->pos().x() < -1.)
		{
			root->setProperty("winner", "zombies");
			emit gameFinished();
			return;
		}
	}

	QList<QVariant> bullets = root->property("bullets").toList();
	for (int i = 0;	i < bullets.length(); )
	{
		QMap<QString, QVariant> bullet(bullets[i].toMap());

		if (bullet["pos"].toPointF().x() >= mapSize.width())
		{
			bullets.removeAt(i);
			continue;
		}

		Zombie *hitZombie = nullptr;
		double hitZombieX = 0;
		for (const QVariant &y : root->property("zombies").toList())
		{
			Zombie *zombie = (Zombie *)(y.value<QPointer<Zombie>>());
			if(zombie->hp() > 0 &&
			   qAbs(zombie->pos().y() - bullet["pos"].toPointF().y()) < 0.5 &&
			   zombie->pos().x() - bullet["pos"].toPointF().x() >= 0 &&
			   zombie->pos().x() - bullet["pos"].toPointF().x() < 0.25 &&
			   !zombie->property("high").toBool() &&
			   (hitZombie == nullptr || zombie->pos().x() < hitZombieX))
			{
				hitZombie = zombie;
				hitZombieX = zombie->pos().x();
			}
		}
		if (hitZombie)
		{
			hitZombie->setHp(hitZombie->hp() - bullet["damage"].toInt());
			if (bullet["type"] == "ice")
				hitZombie->setProperty("frozen", true);
			bullets.removeAt(i);
		}
		else
		{
			bullet["pos"] = bullet["pos"].toPointF() + QPointF((newCurrentTime - oldCurrentTime) / 200., 0.);
			bullets[i] = bullet;
			i++;
		}
	}
	root->setProperty("bullets", bullets);

	if(oldCurrentTime < 1000 && newCurrentTime >= 1000)
	{
		QPointer<Zombie> newZombie = dynamic_cast<Zombie *>(GetZombieClassByID(3)->newInstance());
		newZombie->setPos(QPointF(8, qrand() % 5));

		QList<QVariant> zombiesData(root->property("zombies").toList());
		zombiesData.append(QVariant::fromValue(newZombie));
		root->setProperty("zombies", zombiesData);
	}
}
