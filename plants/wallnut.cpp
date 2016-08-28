#include "wallnut.h"

WallNut::WallNut(QObject *parent) :
	Plant(parent)
{

}

QString WallNut::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/WallNut/WallNut.gif");
}
