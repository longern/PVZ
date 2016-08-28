#ifndef WALLNUT_H
#define WALLNUT_H

#include "plant.h"

class WallNut : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/WallNut/0.gif")

public:
	Q_INVOKABLE WallNut(QObject *parent = 0);
	virtual QString imgSrc() const;
};

#endif // WALLNUT_H
