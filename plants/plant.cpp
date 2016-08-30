#include "plant.h"
#include <QElapsedTimer>
#include <QList>

Plant::Plant(QObject *parent) :
	QObject(parent)
{
	mHealthPoint = 300;
	mCoolDown = 7500;
}

bool Plant::canPlant(QObject *root)
{
	if (root->property("sunvalue").toInt() < cost())
		return false;

	qint64 currentTime = root->property("currentTime").toLongLong();
	QList<QVariant> plantsData(root->property("plants").toList());
	for (const QVariant &x : plantsData)
	{
		QPointer<Plant> plant = x.value<QPointer<Plant>>();
		if (plant->pos() == mPlantPosition ||
			(plant->metaObject()->className() == metaObject()->className() &&
			currentTime - plant->property("plantTime").toLongLong() < mCoolDown))
			return false;
	}
	return true;
}

void Plant::onPlanted(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	setProperty("plantTime", newCurrentTime);
	root->setProperty("sunvalue", root->property("sunvalue").toInt() - cost());
}

void Plant::onTimeout(QObject *root)
{
	if (mHealthPoint <= 0.)
	{
		onRemoved(root);
	}
}

void Plant::onRemoved(QObject *root)
{
	deleteLater();
	QList<QVariant> plantsData(root->property("plants").toList());
	plantsData.removeOne(QVariant::fromValue(QPointer<Plant>(this)));
	root->setProperty("plants", plantsData);
}
