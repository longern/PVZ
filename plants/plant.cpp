#include "plant.h"

Plant::Plant(QObject *parent) :
	QObject(parent)
{
}

bool Plant::canPlant(QObject *root)
{
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
	Q_UNUSED(root)
}

void Plant::onTimeout(QObject *root)
{
	Q_UNUSED(root)
}
