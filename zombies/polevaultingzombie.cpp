#include "polevaultingzombie.h"
#include "plants/plant.h"

PoleVaultingZombie::PoleVaultingZombie(QObject *parent) :
	Zombie(parent)
{
	mHealthPoint = 500;
	mMoveSpeed = 1. / 2350.;
	setProperty("state", "running");
}

QString PoleVaultingZombie::imgSrc() const
{
	QString zombieState(property("state").toString());
	if (zombieState == "running")
		return QStringLiteral(":/zombies/images/Zombies/PoleVaultingZombie/PoleVaultingZombie.gif");
	else if(zombieState == "jumping")
		return QStringLiteral(":/zombies/images/Zombies/PoleVaultingZombie/PoleVaultingZombieJump.gif");
	else if(zombieState == "jumping2")
		return QStringLiteral(":/zombies/images/Zombies/PoleVaultingZombie/PoleVaultingZombieJump2.gif");
	else if(zombieState == "attacking")
		return QStringLiteral(":/zombies/images/Zombies/PoleVaultingZombie/PoleVaultingZombieAttack.gif");
	else
		return QStringLiteral(":/zombies/images/Zombies/PoleVaultingZombie/PoleVaultingZombieWalk.gif");
}

void PoleVaultingZombie::onTimeout(QObject *root)
{
	if (mHealthPoint <= 0.)
	{
		deleteLater();
		QList<QVariant> zombiesData(root->property("zombies").toList());
		zombiesData.removeOne(QVariant::fromValue(QPointer<Zombie>(this)));
		root->setProperty("zombies", zombiesData);
	}

	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	QString zombieState(property("state").toString());

	if (zombieState == "running")
	{
		for (const QVariant &x : root->property("plants").toList())
		{
			Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
			if(plant->pos().y() == mZombiePosition.y() &&
			   mZombiePosition.x() - plant->pos().x() >= 0 &&
			   mZombiePosition.x() - plant->pos().x() < 0.5)
			{
				setProperty("state", QStringLiteral("jumping"));
				setProperty("jumpTime", newCurrentTime);
				setProperty("high", true);
				return;
			}
		}
		mZombiePosition.rx() -= (newCurrentTime - oldCurrentTime) * moveSpeed();
		return;
	}

	if (zombieState == "jumping")
	{
		if (newCurrentTime - property("jumpTime").toLongLong() >= 500)
		{
			setProperty("state", QStringLiteral("jumping2"));
			mZombiePosition += QPointF(-1., 0.);
		}
		return;
	}

	if (zombieState == "jumping2")
	{
		if (newCurrentTime - property("jumpTime").toLongLong() >= 1000)
		{
			setProperty("state", QStringLiteral("moving"));
			setProperty("high", false);
			mMoveSpeed /= 2;
		}
		return;
	}

	for (const QVariant &x : root->property("plants").toList())
	{
		Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
		if (plant->pos().y() == mZombiePosition.y() &&
		   mZombiePosition.x() - plant->pos().x() >= 0 &&
		   mZombiePosition.x() - plant->pos().x() < 0.5)
		{
			setProperty("state", QStringLiteral("attacking"));
			plant->setHp(plant->hp() - (newCurrentTime - oldCurrentTime) * attackDamage() / 1000.);
			return;
		}
	}

	setProperty("state", QStringLiteral("moving"));
	mZombiePosition.rx() -= (newCurrentTime - oldCurrentTime) * moveSpeed();
}
