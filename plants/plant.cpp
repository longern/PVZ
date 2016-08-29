#include "plant.h"
#include <QElapsedTimer>

Plant::Plant(QObject *parent) :
	QObject(parent)
{
	mHealthPoint = 300;
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

void Plant::onPlanted(QObject *)
{
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	setProperty("plantTime", QVariant(elapsedTimer.msecsSinceReference()));
}

void Plant::onTimeout(QObject *root)
{
	Q_UNUSED(root)
}
