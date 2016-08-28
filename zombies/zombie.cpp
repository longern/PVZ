#include "zombie.h"

Zombie::Zombie(QObject *parent) :
	QObject(parent)
{

}

void Zombie::onTimeout(QObject *root)
{
	Q_UNUSED(root);
}
