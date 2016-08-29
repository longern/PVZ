#include "plants.h"

const QMetaObject *GetPlantClassByID(int id)
{
	switch (id)
	{
	case 1:
		return &Peashooter::staticMetaObject;
	case 2:
		return &SunFlower::staticMetaObject;
	case 3:
		return &WallNut::staticMetaObject;
	case 4:
		return &SnowPea::staticMetaObject;
	case 5:
		return &CherryBomb::staticMetaObject;
	default:
		return 0;
	}
}
