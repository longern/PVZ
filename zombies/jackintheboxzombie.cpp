#include "jackintheboxzombie.h"
#include "plants/plant.h"

JackInTheBoxZombie::JackInTheBoxZombie(QObject *parent) :
	Zombie(parent)
{
	mHealthPoint = 1370.;
	mMoveSpeed = 1. / 2200.;
}

QString JackInTheBoxZombie::imgSrc() const
{
	if (property("state").toString() == "openBox")
		return QStringLiteral(":/zombies/images/Zombies/JackinTheBoxZombie/OpenBox.gif");
	else
		return QStringLiteral(":/zombies/images/Zombies/JackinTheBoxZombie/Walk.gif");
}

void JackInTheBoxZombie::onTimeout(QObject *root)
{
	if (mHealthPoint <= 70.)
	{
		Zombie::onTimeout(root);
		return;
	}

	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();

	if (property("state").toString() == "moving")
	{
		for (const QVariant &x : root->property("plants").toList())
		{
			Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
			if (plant->pos().y() == mZombiePosition.y() &&
			   mZombiePosition.x() - plant->pos().x() >= 0 &&
			   mZombiePosition.x() - plant->pos().x() < 0.5)
			{
				setProperty("state", QStringLiteral("openBox"));
				setProperty("openBoxTime", newCurrentTime);
				return;
			}
		}
		mZombiePosition.rx() -= (newCurrentTime - oldCurrentTime) * moveSpeed();
	}

	if (property("state").toString() == "openBox")
	{
		if (newCurrentTime - property("openBoxTime").toLongLong() >= 1500 &&
			oldCurrentTime - property("openBoxTime").toLongLong() < 1500)
		{
			for (const QVariant &x : root->property("plants").toList())
			{
				Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
				if (sqrt(pow(plant->pos().y() - mZombiePosition.y(), 2) + pow(mZombiePosition.x() - plant->pos().x(), 2)) <= 1.81)
					plant->setHp(plant->hp() - 1800);
			}
			deleteLater();
			QList<QVariant> zombiesData(root->property("zombies").toList());
			zombiesData.removeOne(QVariant::fromValue(QPointer<Zombie>(this)));
			root->setProperty("zombies", zombiesData);
		}
	}
}
