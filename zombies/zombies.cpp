#include "zombies.h"

const QMetaObject *GetZombieClassByID(int id)
{
	switch (id)
	{
	case 1:
		return &BasicZombie::staticMetaObject;
	default:
		return 0;
	}
}
