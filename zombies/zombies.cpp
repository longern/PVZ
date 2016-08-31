#include "zombies.h"

const QMetaObject *GetZombieClassByID(int id)
{
	static QList<const QMetaObject *> zombieClasses = {
		&BasicZombie::staticMetaObject,
		&ConeheadZombie::staticMetaObject,
		&PoleVaultingZombie::staticMetaObject,
		&BucketheadZombie::staticMetaObject
	};

	if (id >= 1 && id <= zombieClasses.length())
		return zombieClasses.at(id - 1);
	return 0;
}
