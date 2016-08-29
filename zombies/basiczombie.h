#ifndef BASICZOMBIE_H
#define BASICZOMBIE_H

#include "zombie.h"

class BasicZombie : public Zombie
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/zombies/images/Zombies/Zombie/Zombie.gif")

public:
	Q_INVOKABLE BasicZombie(QObject *parent = 0);
	virtual QString imgSrc() const;
};

#endif // BASICZOMBIE_H
