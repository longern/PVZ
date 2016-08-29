#include "zombie.h"

Zombie::Zombie(QObject *parent) :
	QObject(parent)
{

}

void Zombie::onTimeout(QObject *root)
{
	QElapsedTimer elapsedTimer;
	elapsedTimer.start();
	qint64 newCurrentTime = root->property("currentTime").toLongLong();
	qint64 oldCurrentTime = root->property("lastFrameTime").toLongLong();
	mZombiePosition.rx() -= (newCurrentTime - oldCurrentTime) * mMoveSpeed;
}
