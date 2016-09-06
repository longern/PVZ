#include "cherrybomb.h"
#include "zombies/zombie.h"

CherryBomb::CherryBomb(QObject *parent) :
	Plant(parent)
{
	mAttackDamage = 1800;
	mAttackSpeed = 1200;
	mCost = 150;
	mCoolDown = 50000;
}

QString CherryBomb::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/CherryBomb/CherryBomb.gif");
}

void CherryBomb::onTimeout(QObject *root)
{
	qint64 currentTime = root->property("currentTime").toLongLong();
	qint64 plantTime = property("plantTime").toLongLong();
	if (currentTime - plantTime > mAttackSpeed)
	{
		for (const QVariant &y : root->property("zombies").toList())
		{
			Zombie *zombie = (Zombie *)(y.value<QPointer<Zombie>>());
			if(zombie->hp() > 0 &&
			   sqrt(pow(zombie->pos().x() - pos().x(), 2) + pow(zombie->pos().y() - pos().y(), 2)) <= 1.81)
			{
				zombie->setHp(zombie->hp() - mAttackDamage);
				zombie->setProperty("state", "bombed");
				zombie->setProperty("bombedTime", currentTime);
			}
		}
		deleteLater();
		QList<QVariant> plantsData(root->property("plants").toList());
		plantsData.removeOne(QVariant::fromValue(QPointer<Plant>(this)));
		root->setProperty("plants", plantsData);
	}
}
