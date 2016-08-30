#include "zombies.h"

const QMetaObject *GetZombieClassByID(int id)
{
	switch (id)
	{
	case 1:
		return &BasicZombie::staticMetaObject;
	case 2:
		return &PoleVaultingZombie::staticMetaObject;
	case 3:
		return &BucketheadZombie::staticMetaObject;
	default:
		return 0;
	}
}
