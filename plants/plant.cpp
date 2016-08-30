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
	QMap<QString, QVariant> lastPlantTime = root->property("lastPlantTime").toMap();
	if (!lastPlantTime[metaObject()->className()].isNull() &&
		currentTime - lastPlantTime[metaObject()->className()].toLongLong() < mCoolDown)
		return false;
	root->setProperty("lastPlantTime", lastPlantTime);
	return true;
}

void Plant::onPlanted(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	setProperty("plantTime", newCurrentTime);
	QMap<QString, QVariant> lastPlantTime = root->property("lastPlantTime").toMap();
	lastPlantTime[metaObject()->className()] = newCurrentTime;
	root->setProperty("lastPlantTime", lastPlantTime);
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
