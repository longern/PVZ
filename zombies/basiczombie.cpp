#include "basiczombie.h"

BasicZombie::BasicZombie(QObject *parent) :
	Zombie(parent)
{

}

QString BasicZombie::imgSrc() const
{
	return QStringLiteral(":/zombies/images/Zombies/Zombie/Zombie.gif");
}
