#include "plant.h"
#include <QElapsedTimer>
#include <QList>

Plant::Plant(QObject *parent) :
	QObject(parent)
{
	mHealthPoint = 300;
}

bool Plant::canPlant(QObject *root)
{
	if (root->property("sunshine").toInt() < cost())
		return false;

	QList<QVariant> plantsData(root->property("plants").toList());
	for (const QVariant &x : plantsData)
	{
		QPointer<Plant> plant = x.value<QPointer<Plant>>();
		if (plant)
			if (plant->pos() == mPlantPosition)
				return false;
	}
	return true;
}

void Plant::onPlanted(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	setProperty("plantTime", newCurrentTime);
	root->setProperty("sunshine", root->property("sunshine").toInt() - cost());
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
