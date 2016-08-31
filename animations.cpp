#include "ui_playform.h"
#include "playinginterface.h"
#include "gamelogic.h"
#include <QPropertyAnimation>

QVariant boolInterpolator(const bool &start, const bool &end, qreal progress)
{
	return progress < 1. ? start : end;
}

void PlayingInterface::registerInterpolator()
{
	qRegisterAnimationInterpolator<bool>(boolInterpolator);
}

void PlayingInterface::onAnimationFinished()
{
	int animationID = 0;
	if (QObject::sender() != nullptr)
		animationID = QObject::sender()->property("animationID").toInt();
	QAbstractAnimation *ani = nullptr;
	switch (animationID)
	{
	case 0:
	{
		QSequentialAnimationGroup *seqAni = new QSequentialAnimationGroup(this);
		QPropertyAnimation *propAni;
		seqAni->addPause(1500);
		propAni = new QPropertyAnimation(ui->widgetBackground, "pos", this);
		propAni->setDuration(2000);
		propAni->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
		propAni->setKeyValueAt(0., QPoint(0, 0));
		propAni->setKeyValueAt(1., QPoint(-500, 0));
		seqAni->addAnimation(propAni);
		propAni = new QPropertyAnimation(ui->widgetCardBack, "pos", this);
		propAni->setDuration(250);
		propAni->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
		propAni->setKeyValueAt(0., QPoint(0, -85));
		propAni->setKeyValueAt(1., QPoint(0, 0));
		seqAni->addAnimation(propAni);
		seqAni->addPause(2000);
		propAni = new QPropertyAnimation(ui->widgetBackground, "pos", this);
		propAni->setDuration(1500);
		propAni->setEasingCurve(QEasingCurve(QEasingCurve::InOutCubic));
		propAni->setKeyValueAt(0., QPoint(-500, 0));
		propAni->setKeyValueAt(1., QPoint(-180, 0));
		seqAni->addAnimation(propAni);
		ani = seqAni;
		break;
	}
	case 1:
	{
		mGameLogic->onGameStart(mGameStatus);
		return;
	}
	}
	ani->setProperty("animationID", animationID + 1);
	connect(ani, SIGNAL(finished()), this, SLOT(onAnimationFinished()));
	ani->start(QAbstractAnimation::DeleteWhenStopped);
}
