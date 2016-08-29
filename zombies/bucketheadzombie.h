#ifndef BUCKETHEADZOMBIE_H
#define BUCKETHEADZOMBIE_H

#include "zombie.h"

class BucketheadZombie : public Zombie
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/zombies/images/Zombies/BucketheadZombie/0.gif")

public:
	Q_INVOKABLE BucketheadZombie(QObject *parent = 0);
	virtual QString imgSrc() const;
};

#endif // BUCKETHEADZOMBIE_H
