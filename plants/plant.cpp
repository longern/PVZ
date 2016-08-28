#include "plant.h"

Plant::Plant(QObject *parent) :
	QObject(parent)
{
	mPlantIndex = -1;
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
