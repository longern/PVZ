#ifndef POTATOMINE_H
#define POTATOMINE_H

#include "plant.h"

class PotatoMine : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/PotatoMine/0.gif")

public:
	Q_INVOKABLE PotatoMine(QObject *parent = 0);
	virtual QString imgSrc() const;

	Q_INVOKABLE virtual void onTimeout(QObject *root);
};

#endif // POTATOMINE_H
