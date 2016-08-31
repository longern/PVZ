#ifndef ZOMBIES
#define ZOMBIES

#include "basiczombie.h"
#include "coneheadzombie.h"
#include "polevaultingzombie.h"
#include "bucketheadzombie.h"
#include "jackintheboxzombie.h"

const QMetaObject *GetZombieClassByID(int id);

#endif // ZOMBIES
