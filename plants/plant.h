#ifndef PLANT_H
#define PLANT_H

#include <QtCore>

class Plant : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QPointF pos READ pos WRITE setPos)
	Q_PROPERTY(double hp READ hp WRITE setHp)
	Q_PROPERTY(int attackDamage READ attackDamage)
	Q_PROPERTY(int attackSpeed READ attackSpeed)
	Q_PROPERTY(int cd READ cd)
	Q_PROPERTY(int cost READ cost)

public:
	explicit Plant(QObject *parent = 0);
	~Plant() { }
	virtual QString imgSrc() const = 0;

	QPointF pos() const { return mPlantPosition; }
	void setPos(QPointF newPos) { mPlantPosition = newPos; }
	double hp() const { return mHealthPoint; }
	void setHp(double hp) { mHealthPoint = hp; }
	int attackDamage() const { return mAttackDamage; }
	int attackSpeed() const { return mAttackSpeed; }
	int cd() { return mCoolDown; }
	int cost() { return mCost; }

	Q_INVOKABLE virtual bool canPlant(QObject *root);
	Q_INVOKABLE virtual void onPlanted(QObject *root);
	Q_INVOKABLE virtual void onTimeout(QObject *root);
	Q_INVOKABLE virtual void onRemoved(QObject *root);

protected:
	QPointF mPlantPosition;
	double mHealthPoint;
	int mAttackDamage;
	int mAttackSpeed;
	int mCoolDown;
	int mCost;
};

#endif // PLANT_H
