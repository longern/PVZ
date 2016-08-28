#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QtCore>

class Zombie : public QObject
{
	Q_PROPERTY(int hp READ hp WRITE setHp NOTIFY hpChanged)
	Q_PROPERTY(int attack READ attack WRITE setAttack NOTIFY attackChanged)
	Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)

public:
	Zombie();
};

#endif // ZOMBIE_H
