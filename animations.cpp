#include "ui_playform.h"
#include "playinginterface.h"
#include "gamelogic.h"
#include "zombies/zombie.h"
#include "sound.h"
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
		propAni->setEasingCurve(QEasingCurve::InOutCubic);
		propAni->setKeyValueAt(0., QPoint(0, 0));
		propAni->setKeyValueAt(1., QPoint(-500, 0));
		seqAni->addAnimation(propAni);
		propAni = new QPropertyAnimation(ui->widgetCardBack, "pos", this);
		propAni->setDuration(250);
		propAni->setEasingCurve(QEasingCurve::InOutCubic);
		propAni->setKeyValueAt(0., QPoint(0, -85));
		propAni->setKeyValueAt(1., QPoint(0, 0));
		seqAni->addAnimation(propAni);
		seqAni->addPause(2000);
		propAni = new QPropertyAnimation(ui->widgetBackground, "pos", this);
		propAni->setDuration(1500);
		propAni->setEasingCurve(QEasingCurve::InOutCubic);
		propAni->setKeyValueAt(0., QPoint(-500, 0));
		propAni->setKeyValueAt(1., QPoint(-180, 0));
		connect(propAni, &QPropertyAnimation::finished, Sound::ins(), &Sound::stop);
		seqAni->addAnimation(propAni);
		connect(seqAni->addPause(500), &QPauseAnimation::finished, [this]() { ui->labelReady->show(); });
		connect(seqAni->addPause(1000), &QPauseAnimation::finished, [this]() { ui->labelReady->hide(); ui->labelSet->show(); });
		connect(seqAni->addPause(1000), &QPauseAnimation::finished, [this]() { ui->labelSet->hide(); ui->labelPlant->show(); });
		connect(seqAni->addPause(1000), &QPauseAnimation::finished, [this]() { ui->labelPlant->hide(); });
		ani = seqAni;
		break;
	}
	case 1:
	{
		mGameLogic->onGameStart(mGameStatus);
		Sound::ins()->play("qrc:/music/audio/Kitanai Sekai.mp3");
		return;
	}
	}
	ani->setProperty("animationID", animationID + 1);
	connect(ani, SIGNAL(finished()), this, SLOT(onAnimationFinished()));
	ani->start(QAbstractAnimation::DeleteWhenStopped);
}

QPoint PlayingInterface::zombieAnimationOffset(Zombie *zombie)
{
	static QMap<QString, QPoint> zombieAnimationOffset;
	if (zombieAnimationOffset.isEmpty())
	{
		zombieAnimationOffset["BasicZombie"] = QPoint(-65, -110);
		zombieAnimationOffset["ConeheadZombie"] = QPoint(-85, -110);
		zombieAnimationOffset["PoleVaultingZombie"] = QPoint(-190, -130);
		zombieAnimationOffset["BucketheadZombie"] = QPoint(-65, -110);
		zombieAnimationOffset["JackInTheBoxZombie"] = QPoint(-105, -120);
	}
	if (zombie->property("state") == "bombed")
		return zombieAnimationOffset["BasicZombie"];
	else
		return zombieAnimationOffset[zombie->metaObject()->className()];
}
