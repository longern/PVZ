#include "basiczombie.h"

BasicZombie::BasicZombie(QObject *parent) :
	Zombie(parent)
{
	mAttackDamage = 100;
	mMoveSpeed = 1. / 4700.;
}

QString BasicZombie::imgSrc() const
{
	return QStringLiteral(":/zombies/images/Zombies/Zombie/Zombie.gif");
}
