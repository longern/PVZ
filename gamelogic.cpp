#include "gamelogic.h"
#include "plants/plants.h"
#include "zombies/zombies.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{
	addTimeFlag(1000, [](QObject *root) {
		Zombie *newZombie = dynamic_cast<Zombie *>(GetZombieClassByID(1)->newInstance());
		newZombie->setPos(QPointF(11, qrand() % 5));
		newZombie->onCreated(root);
	});
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

	for (int i = 0; i < timeFlags.size();)
	{
		if(oldCurrentTime < timeFlags[i].first && newCurrentTime >= timeFlags[i].first)
		{
			timeFlags[i].second(root);
			timeFlags.removeAt(i);
		}
		else
			i++;
	}
}

void GameLogic::addTimeFlag(qint64 timePoint, std::function<void (QObject *)> func)
{
	timeFlags.append(qMakePair(timePoint, func));
}
