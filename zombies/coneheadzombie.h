#ifndef CONEHEADZOMBIE_H
#define CONEHEADZOMBIE_H

#include "zombie.h"

class ConeheadZombie : public Zombie
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/zombies/images/Zombies/ConeheadZombie/0.gif")

public:
	Q_INVOKABLE ConeheadZombie(QObject *parent = 0);
	virtual QString imgSrc() const;
};

#endif // CONEHEADZOMBIE_H
