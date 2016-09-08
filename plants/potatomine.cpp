#include "potatomine.h"
#include "zombies/zombie.h"

PotatoMine::PotatoMine(QObject *parent) :
	Plant(parent)
{
	mAttackDamage = 1800;
	mAttackSpeed = 15000;
	mCost = 25;
	mCoolDown = 30000;
}

QString PotatoMine::imgSrc() const
{
	if (property("state").toString() == "ready")
		return QStringLiteral(":/plants/images/Plants/PotatoMine/PotatoMine.gif");
	else
		return QStringLiteral(":/plants/images/Plants/PotatoMine/PotatoMineNotReady.gif");
}

void PotatoMine::onTimeout(QObject *root)
{
	qint64 currentTime = root->property("currentTime").toLongLong();
	qint64 plantTime = property("plantTime").toLongLong();
	if (property("state").toString() == "ready")
	{
		bool activate = false;
		for (const QVariant &y : root->property("zombies").toList())
		{
			Zombie *zombie = (Zombie *)(y.value<QPointer<Zombie>>());
			if(zombie->hp() > 0 &&
			   qAbs(mPlantPosition.y() - zombie->pos().y()) <= 0.5 && qAbs(mPlantPosition.x() - zombie->pos().x()) <= 0.5)
			{
				activate = true;
				break;
			}
		}
		if (activate)
		{
			for (const QVariant &y : root->property("zombies").toList())
			{
				Zombie *zombie = (Zombie *)(y.value<QPointer<Zombie>>());
				if(zombie->hp() > 0 &&
				   qAbs(mPlantPosition.y() - zombie->pos().y()) <= 0.5 && qAbs(mPlantPosition.x() - zombie->pos().x()) <= 1.)
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
	else if (currentTime - plantTime > mAttackSpeed)
	{
		setProperty("state", "ready");
	}
	Plant::onTimeout(root);
}
