#include "sunflower.h"

SunFlower::SunFlower(QObject *parent) :
	Plant(parent)
{

}

QString SunFlower::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/SunFlower/SunFlower1.gif");
}
