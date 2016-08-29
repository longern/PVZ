#include "wallnut.h"

WallNut::WallNut(QObject *parent) :
	Plant(parent)
{
	mHealthPoint = 4000;
}

QString WallNut::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/WallNut/WallNut.gif");
}
