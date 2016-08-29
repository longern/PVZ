#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <QtCore>

class Zombie : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(int hp READ hp WRITE setHp)
	Q_PROPERTY(int attackDamage READ attackDamage)
	Q_PROPERTY(int attackSpeed READ attackSpeed)

public:
	explicit Zombie(QObject *parent = 0);
	~Zombie() { }
	virtual QString imgSrc() const = 0;

	QPointF pos() const { return mZombiePosition; }
	void setPos(QPointF newPos) { mZombiePosition = newPos; }
	int hp() const { return mHealthPoint; }
	void setHp(int hp) { mHealthPoint = hp; }
	int attackDamage() const { return mAttackDamage; }
	int attackSpeed() const { return mAttackSpeed; }

	Q_INVOKABLE virtual void onTimeout(QObject *root);

protected:
	QPointF mZombiePosition;
	int mHealthPoint;
	int mAttackDamage;
	int mAttackSpeed;
};

#endif // ZOMBIE_H
