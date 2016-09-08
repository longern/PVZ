#include "gamelogic.h"
#include "plants/plants.h"
#include "zombies/zombies.h"

GameLogic::GameLogic(QObject *parent) : QObject(parent)
{

}

void GameLogic::startGame(QObject *root)
{
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	root->setProperty("gameStartTime", QVariant(elapsedTimer.msecsSinceReference()));
	root->setProperty("currentTime", QVariant(qint64(0)));
	root->setProperty("defenceTime", 800000);
	if (root->property("mode").toString() != "replay")
	{
		for (int i = 0; i < 40; i++)
		{
			addTimeFlag(i * 20000 + 10000, [i, this](QObject *root) {
				int virus = int(std::pow(i, 1.53) / 2) + 2;
				int offset = 0;
				int lastTrack = -1;
				while (virus > 1)
				{
					int zombieType = qrand() % qMin((virus + 1) / 3, 5) + 1;
					virus -= zombieType * 3 - 1;
					int track;
					do
					{
						track = qrand() % 5;
					} while (track == lastTrack);
					createZombie(root, zombieType, 11. + offset / 10., track);
					lastTrack = track;
					offset++;
				}
			});
		}

		if (root->property("mode").toString() != "lastStand")
		{
			for (int i = 0; i < 80; i++)
			{
				addTimeFlag(10000 * i, [this](QObject *root) {
					QSize mapSize = root->property("mapSize").toSize();
					createSunshine(root, qrand() / (RAND_MAX - 1.) * mapSize.width() - 0.5);
				});
			}
		}
	}
}

void GameLogic::onTimeout(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	QSize mapSize = root->property("mapSize").toSize();

	QList<QVariant> zombies = root->property("zombies").toList();
	if (newCurrentTime > root->property("defenceTime").toInt() && zombies.isEmpty())
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

void GameLogic::createPlant(QObject *root, int plantType, double x, double y)
{
	QPointer<Plant> newPlant = dynamic_cast<Plant *>(GetPlantClassByID(plantType)->newInstance());
	newPlant->setPos(QPointF(x, y));
	if (!newPlant->canPlant(root))
	{
		newPlant->deleteLater();
		return;
	}
	newPlant->onPlanted(root);

	QList<QVariant> plantsData(root->property("plants").toList());
	plantsData.append(QVariant::fromValue(newPlant));
	root->setProperty("plants", plantsData);
}

void GameLogic::createZombie(QObject *root, int zombieType, double x, double y)
{
	Zombie *newZombie = dynamic_cast<Zombie *>(GetZombieClassByID(zombieType)->newInstance());
	newZombie->setProperty("type", zombieType);
	newZombie->setPos(QPointF(x, y));
	newZombie->onCreated(root);
	emit zombieCreated(newZombie);
}

void GameLogic::createSunshine(QObject *root, double x)
{
	QList<QVariant> sunshineList = root->property("sunshineList").toList();
	QMap<QString, QVariant> sunshine;
	sunshine["type"] = QStringLiteral("auto");
	sunshine["value"] = 25;
	sunshine["pos"] = QPointF(x, 0);
	sunshineList.append(sunshine);
	root->setProperty("sunshineList", sunshineList);
	emit sunshineCreated();
}

void GameLogic::collectSunshine(QObject *root, int index)
{
	emit sunshineCollected(index);
	QList<QVariant> sunshineList = root->property("sunshineList").toList();
	QMap<QString, QVariant> sunshine = sunshineList[index].toMap();
	root->setProperty("sunvalue", root->property("sunvalue").toInt() + sunshine["value"].toInt());
	sunshineList.removeAt(index);
	root->setProperty("sunshineList", sunshineList);
}

void GameLogic::addTimeFlag(qint64 timePoint, std::function<void (QObject *)> func)
{
	timeFlags.append(qMakePair(timePoint, func));
}
