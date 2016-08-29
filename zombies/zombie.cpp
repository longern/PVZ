#include "zombie.h"
#include "plants/plants.h"

Zombie::Zombie(QObject *parent) :
	QObject(parent)
{

}

void Zombie::onTimeout(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	for (const QVariant &x : root->property("plants").toList())
	{
		Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
		if(plant->pos().y() == mZombiePosition.y() &&
		   mZombiePosition.x() - plant->pos().x() >= 0 &&
		   mZombiePosition.x() - plant->pos().x() < 0.5)
		{
			plant->setHp(plant->hp() - (newCurrentTime - oldCurrentTime) * mAttackDamage / 1000.);
			return;
		}
	}
	mZombiePosition.rx() -= (newCurrentTime - oldCurrentTime) * mMoveSpeed;
}
