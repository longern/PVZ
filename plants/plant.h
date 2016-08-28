#ifndef PLANT_H
#define PLANT_H

#include <QtCore>

class Plant : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int hp READ hp WRITE setHp)
	Q_PROPERTY(int attackDamage READ attackDamage)
	Q_PROPERTY(int attackSpeed READ attackSpeed)
	Q_PROPERTY(int cd READ cd)
	Q_PROPERTY(int cost READ cost)

public:
	explicit Plant(QObject *parent = 0);
	~Plant() { }
	virtual QString imgSrc() const = 0;

	int plantIndex() const { return mPlantIndex; }
	void setPlantIndex(int index) { mPlantIndex = index; }
	QPoint pos() const { return mPlantPosition; }
	void setPos(QPoint newPos) { mPlantPosition = newPos; }
	int hp() const { return mHealthPoint; }
	void setHp(int hp) { mHealthPoint = hp; }
	int attackDamage() const { return mAttackDamage; }
	int attackSpeed() const { return mAttackSpeed; }
	int cd() { return mCoolDown; }
	int cost() { return mCost; }

	virtual bool canPlant(QObject *root);

protected:
	int mPlantIndex;
	QPoint mPlantPosition;
	int mHealthPoint;
	int mAttackDamage;
	int mAttackSpeed;
	int mCoolDown;
	int mCost;
};

#endif // PLANT_H
