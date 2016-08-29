#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include "plant.h"

class CherryBomb : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/CherryBomb/0.gif")

public:
	Q_INVOKABLE CherryBomb(QObject *parent = 0);
	virtual QString imgSrc() const;

	Q_INVOKABLE virtual void onTimeout(QObject *root);
};

#endif // CHERRYBOMB_H
