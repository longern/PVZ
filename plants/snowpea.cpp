#include "snowPea.h"

SnowPea::SnowPea(QObject *parent) :
	Plant(parent)
{

}

QString SnowPea::imgSrc() const
{
	return QStringLiteral(":/plants/images/Plants/SnowPea/SnowPea.gif");
}
