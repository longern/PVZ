#include "wallnut.h"

WallNut::WallNut(QObject *parent) :
	Plant(parent)
{
	mHealthPoint = 4000.;
}

QString WallNut::imgSrc() const
{
	if (mHealthPoint <= 4000. / 3.)
		return QStringLiteral(":/plants/images/Plants/WallNut/Wallnut_cracked2.gif");
	else if (mHealthPoint <= 4000. * 2. / 3.)
		return QStringLiteral(":/plants/images/Plants/WallNut/Wallnut_cracked1.gif");
	else
		return QStringLiteral(":/plants/images/Plants/WallNut/WallNut.gif");
}
