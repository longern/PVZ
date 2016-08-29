#include "basiczombie.h"

BasicZombie::BasicZombie(QObject *parent) :
	Zombie(parent)
{
	mHealthPoint = 270;
	mAttackDamage = 100;
	mMoveSpeed = 1. / 5700.;
}

QString BasicZombie::imgSrc() const
{
	return QStringLiteral(":/zombies/images/Zombies/Zombie/Zombie.gif");
}
