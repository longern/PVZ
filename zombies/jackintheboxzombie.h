#ifndef JACKINTHEBOXZOMBIE_H
#define JACKINTHEBOXZOMBIE_H

#include "zombie.h"

class JackInTheBoxZombie : public Zombie
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/zombies/images/Zombies/JackInTheBoxZombie/0.gif")

public:
	Q_INVOKABLE JackInTheBoxZombie(QObject *parent = 0);
	virtual QString imgSrc() const;

	Q_INVOKABLE virtual void onTimeout(QObject *root);
};

#endif // JACKINTHEBOXZOMBIE_H
