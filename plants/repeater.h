#ifndef REPEATER_H
#define REPEATER_H

#include "plant.h"

class Repeater : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/Repeater/0.gif")

public:
	Q_INVOKABLE Repeater(QObject *parent = 0);
	virtual QString imgSrc() const;

	Q_INVOKABLE virtual void onTimeout(QObject *root);
};

#endif // REPEATER_H
