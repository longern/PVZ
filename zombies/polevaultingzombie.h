#ifndef POLEVAULTINGZOMBIE_H
#define POLEVAULTINGZOMBIE_H

#include "zombie.h"

class PoleVaultingZombie : public Zombie
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/zombies/images/Zombies/PoleVaultingZombie/0.gif")

public:
	Q_INVOKABLE PoleVaultingZombie(QObject *parent = 0);
	virtual QString imgSrc() const;

	Q_INVOKABLE virtual void onTimeout(QObject *root);
};

#endif // POLEVAULTINGZOMBIE_H
