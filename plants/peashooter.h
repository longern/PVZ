#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "plant.h"

class Peashooter : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/Peashooter/0.gif")

public:
	Q_INVOKABLE Peashooter(QObject *parent = 0);
	virtual QString imgSrc() const;

	static QString staticImageSource() { return QStringLiteral(":/plants/images/Plants/Peashooter/0.gif"); }
};

#endif // PEASHOOTER_H
