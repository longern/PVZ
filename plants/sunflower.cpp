#include "sunflower.h"

SunFlower::SunFlower(QObject *parent) :
	Plant(parent)
{
	mCost = 50;
}

QString SunFlower::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/SunFlower/SunFlower1.gif");
}

bool SunFlower::canPlant(QObject *root)
{
	if (root->property("mode").toString() == "lastStand")
		return false;
	return Plant::canPlant(root);
}

void SunFlower::onTimeout(QObject *root)
{
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	qint64 plantTime = property("plantTime").toLongLong();
	const int mProduceSpeed = 24000;
	if ((newCurrentTime - plantTime + mProduceSpeed / 2) / mProduceSpeed >
		(oldCurrentTime - plantTime + mProduceSpeed / 2) / mProduceSpeed)
	{
		QList<QVariant> sunshineList = root->property("sunshineList").toList();
		QMap<QString, QVariant> sunshine;
		sunshine["type"] = QStringLiteral("normal");
		sunshine["value"] = 25;
		sunshine["pos"] = mPlantPosition;
		sunshineList.append(sunshine);
		root->setProperty("sunshineList", sunshineList);
	}
	Plant::onTimeout(root);
}
