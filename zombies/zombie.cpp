#include "zombie.h"
#include "plants/plants.h"

Zombie::Zombie(QObject *parent) :
	QObject(parent)
{
	mHealthPoint = 270;
	setProperty("state", QStringLiteral("moving"));
}

void Zombie::onTimeout(QObject *root)
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

	for (const QVariant &x : root->property("plants").toList())
	{
		Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
		if(plant->pos().y() == mZombiePosition.y() &&
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
