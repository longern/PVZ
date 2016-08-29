#include "basiczombie.h"

BasicZombie::BasicZombie(QObject *parent) :
	Zombie(parent)
{
	mMoveSpeed = 1. / 4700.;
}

QString BasicZombie::imgSrc() const
{
	if (property("state").toString() == "attacking")
		return QStringLiteral(":/zombies/images/Zombies/Zombie/ZombieAttack.gif");
	else
		return QStringLiteral(":/zombies/images/Zombies/Zombie/Zombie.gif");
}
