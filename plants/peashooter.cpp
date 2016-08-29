#include "peashooter.h"

Peashooter::Peashooter(QObject *parent) :
	Plant(parent)
{
	mAttackDamage = 20;
	mAttackSpeed = 1400;
	mCost = 100;
}

QString Peashooter::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/Peashooter/Peashooter.gif");
}

void Peashooter::onTimeout(QObject *root)
{
}
