#ifndef SNOWPEA_H
#define SNOWPEA_H

#include "plant.h"

class SnowPea : public Plant
{
	Q_OBJECT
	Q_CLASSINFO("staticImageSource", ":/plants/images/Plants/SnowPea/0.gif")

public:
	Q_INVOKABLE SnowPea(QObject *parent = 0);
	virtual QString imgSrc() const;
};

#endif // SNOWPEA_H
