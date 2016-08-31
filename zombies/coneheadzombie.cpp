#include "coneheadzombie.h"

ConeheadZombie::ConeheadZombie(QObject *parent) :
	Zombie(parent)
{
	mHealthPoint = 640;
	mMoveSpeed = 1. / 4700.;
}

QString ConeheadZombie::imgSrc() const
{
	if (mHealthPoint > 270)
	{
		if (property("state").toString() == "attacking")
			return QStringLiteral(":/zombies/images/Zombies/ConeheadZombie/ConeheadZombieAttack.gif");
		else
			return QStringLiteral(":/zombies/images/Zombies/ConeheadZombie/ConeheadZombie.gif");
	}
	else
	{
		if (property("state").toString() == "attacking")
			return QStringLiteral(":/zombies/images/Zombies/Zombie/ZombieAttack.gif");
		else
			return QStringLiteral(":/zombies/images/Zombies/Zombie/Zombie.gif");
	}
}
