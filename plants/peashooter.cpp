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
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	qint64 plantTime = property("plantTime").toLongLong();
	if ((newCurrentTime - plantTime) / mAttackSpeed > (oldCurrentTime - plantTime) / mAttackSpeed)
	{
		QList<QVariant> bullets = root->property("bullets").toList();
		QMap<QString, QVariant> bullet;
		bullet["type"] = QStringLiteral("normal");
		bullet["pos"] = mPlantPosition + QPointF(0.3, 0);
		bullet["damage"] = 20;
		bullet["imgSrc"] = QStringLiteral(":/plants/images/Plants/BulletNormal.gif");
		bullets.append(bullet);
		root->setProperty("bullets", bullets);
	}
	Plant::onTimeout(root);
}
