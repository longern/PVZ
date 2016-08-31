#include "gamelogic.h"
#include "plants/plants.h"
#include "zombies/zombies.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{
	for (int i = 0; i < 30; i++)
	{
		addTimeFlag(i * 20000 + 10000, [i](QObject *root) {
			int virus = int(std::pow(i, 1.5) / 2) + 2;
			int offset = 0;
			int lastTrack = -1;
			while (virus > 1)
			{
				int zombieType = qrand() % qMin((virus + 1) / 3, 5) + 1;
				virus -= zombieType * 3 - 1;
				Zombie *newZombie = dynamic_cast<Zombie *>(GetZombieClassByID(zombieType)->newInstance());
				int track;
				do
				{
					track = qrand() % 5;
				} while (track == lastTrack);
				newZombie->setPos(QPointF(11. + offset / 10., track));
				lastTrack = track;
				newZombie->onCreated(root);
				offset++;
			}
		});
	}

	for (int i = 0; i < 60; i++)
	{
		addTimeFlag(10000 * i, [](QObject *root) {
			QSize mapSize = root->property("mapSize").toSize();
			QList<QVariant> sunshineList = root->property("sunshineList").toList();
			QMap<QString, QVariant> sunshine;
			sunshine["type"] = QStringLiteral("auto");
			sunshine["value"] = 25;
			sunshine["pos"] = QPointF(qrand() / (RAND_MAX - 1.) * mapSize.width() - 0.5 , 0);
			sunshineList.append(sunshine);
			root->setProperty("sunshineList", sunshineList);
		});
	}
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

	QList<QVariant> zombies = root->property("zombies").toList();
	if (newCurrentTime > 620000 && zombies.isEmpty())
	{
		root->setProperty("winner", "plants");
		emit gameFinished();
	}
	for (const QVariant &y : zombies)
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

		if (bullet["pos"].toPointF().x() >= mapSize.width() + 1)
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

	QList<QVariant> sunshineList = root->property("sunshineList").toList();
	for (QVariant &x : sunshineList)
	{
		QMap<QString, QVariant> sunshine = x.toMap();
		if (sunshine["type"].toString() == "auto" && sunshine["pos"].toPointF().y() < mapSize.height() - 1.)
		{
			sunshine["pos"] = sunshine["pos"].toPointF() + QPointF(0, (newCurrentTime - oldCurrentTime) / 1000. * 0.3);
			x = sunshine;
		}
	}
	root->setProperty("sunshineList", sunshineList);

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
