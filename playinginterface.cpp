#include "playinginterface.h"
#include "ui_playform.h"
#include "gamelogic.h"
#include "plants/plants.h"
#include "zombies/zombies.h"
#include <QPropertyAnimation>

PlayingInterface::PlayingInterface(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlayForm)
{
	ui->setupUi(this);
	ui->labelPlantWin->hide();
	ui->labelZombieWin->hide();
	ui->labelShovel->setAttribute(Qt::WA_TransparentForMouseEvents);

	for (int i = 1; i <= 5; i++)
	{
		PlantCard *card = ui->widgetCardArea->findChild<PlantCard *>("widgetPlantCard" + QString::number(i));
		const QMetaObject *plantClass = GetPlantClassByID(i);
		if (plantClass && card)
		{
			Plant *plant = dynamic_cast<Plant *>(GetPlantClassByID(i)->newInstance());
			int cost = plant->cost();
			delete plant;
			card->setPlantAvatar(plantClass->classInfo(plantClass->indexOfClassInfo("staticImageSource")).value());
			card->setPlantCost(cost);
		}
	}

	mGameStatus = new QObject(this);
	mGameStatus->setProperty("mapSize", QSize(9, 5));
	mGameStatus->setProperty("sunvalue", 200);
	mGameLogic = new GameLogic(this);
	connect(mGameLogic, SIGNAL(gameFinished()), this, SLOT(onGameFinished()));

	registerInterpolator();
	onAnimationFinished();  // Activate first animation

	timerId = startTimer(0);
}

QLabel *PlayingInterface::createDynamicImage(const QString &imgSrc, QWidget *parent)
{
	QLabel *movieLabel = new QLabel(parent);
	QMovie *movie = new QMovie(imgSrc, QByteArray(), parent);
	movieLabel->setMovie(movie);
	movie->start();
	movieLabel->show();
	return movieLabel;
}

void PlayingInterface::paintEvent(QPaintEvent *)
{
	if (timerId == 0)
		return;

	QSize mapSize = mGameStatus->property("mapSize").toSize();
	QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());

	for (const QVariant &x : mGameStatus->property("plants").toList())
	{
		Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
		connect(plant, SIGNAL(destroyed(QObject*)), this, SLOT(onCreatureDestroyed(QObject*)), Qt::UniqueConnection);
		QLabel *plantMovieLabel;
		if (plant->property("img").isNull())
		{
			plantMovieLabel = createDynamicImage(plant->imgSrc(), ui->widgetLawnArea);
			plant->setProperty("img", QVariant::fromValue(QPointer<QLabel>(plantMovieLabel)));
			plantMovieLabel->resize(112, 81);
			plantMovieLabel->move(plant->pos().x() * cellSize.width(),
								  plant->pos().y() * cellSize.height() - 10);
			plantMovieLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
			plantMovieLabel->show();
		}
		else
			plantMovieLabel = (QLabel *)(plant->property("img").value<QPointer<QLabel>>());
		if (plant->imgSrc() != plantMovieLabel->movie()->fileName())
		{
			plantMovieLabel->movie()->deleteLater();
			QMovie *movie = new QMovie(plant->imgSrc(), QByteArray(), this);
			movie->start();
			plantMovieLabel->setMovie(movie);
		}
	}

	QList<QVariant> zombies = mGameStatus->property("zombies").toList();
	qSort(zombies.begin(), zombies.end(), [](const QVariant &a, const QVariant &b) {
		Zombie *zombieA = (Zombie *)(a.value<QPointer<Zombie>>());
		Zombie *zombieB = (Zombie *)(b.value<QPointer<Zombie>>());
		return zombieA->pos().y() < zombieB->pos().y();
	});
	for (const QVariant &x : zombies)
	{
		Zombie *zombie = (Zombie *)(x.value<QPointer<Zombie>>());
		connect(zombie, SIGNAL(destroyed(QObject*)), this, SLOT(onCreatureDestroyed(QObject*)), Qt::UniqueConnection);
		QLabel *zombieMovieLabel;
		if (zombie->property("img").isNull())
		{
			zombieMovieLabel = createDynamicImage(zombie->imgSrc(), this);
			zombie->setProperty("img", QVariant::fromValue(QPointer<QLabel>(zombieMovieLabel)));
			zombieMovieLabel->resize(348, 218);
			zombieMovieLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
			zombieMovieLabel->show();
		}
		else
			zombieMovieLabel = (QLabel *)(zombie->property("img").value<QPointer<QLabel>>());
		if (zombie->imgSrc() != zombieMovieLabel->movie()->fileName())
		{
			zombieMovieLabel->movie()->deleteLater();
			QMovie *movie = new QMovie(zombie->imgSrc(), QByteArray(), this);
			movie->start();
			zombieMovieLabel->setMovie(movie);
		}
		static QMap<QString, QPoint> zombieAnimationOffset;
		if (zombieAnimationOffset.isEmpty())
		{
			zombieAnimationOffset["BasicZombie"] = QPoint(-65, -110);
			zombieAnimationOffset["ConeheadZombie"] = QPoint(-85, -110);
			zombieAnimationOffset["PoleVaultingZombie"] = QPoint(-190, -130);
			zombieAnimationOffset["BucketheadZombie"] = QPoint(-65, -110);
		}
		zombieMovieLabel->movie()->setSpeed(zombie->property("frozen").toBool() ? 50 : 100);
		zombieMovieLabel->move(QPoint(ui->widgetLawnArea->x() + zombie->pos().x() * cellSize.width(),
							   ui->widgetLawnArea->y() + zombie->pos().y() * cellSize.height()) +
							   zombieAnimationOffset[zombie->metaObject()->className()]);
		zombieMovieLabel->raise();
	}
	ui->labelSunValue->setText(QString::number(mGameStatus->property("sunvalue").toInt()));

	qint64 newCurrentTime = mGameStatus->property("currentTime").toLongLong();
	QList<QVariant> bullets = mGameStatus->property("bullets").toList();
	for (int i = 0; i < bullets.length(); i++)
	{
		QMap<QString, QVariant> bullet = bullets[i].toMap();
		QLabel *bulletLabel;
		if (bullet["img"].isNull())
		{
			bulletLabel = new QLabel(this);
			bulletLabel->setObjectName("bullet");
			bullet["img"] = QVariant::fromValue(QPointer<QLabel>(bulletLabel));
			bulletLabel->setPixmap(bullet["imgSrc"].toString());
			bulletLabel->resize(56, 34);
			bulletLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
			bulletLabel->show();
			bullets[i] = bullet;
		}
		else
			bulletLabel = (QLabel *)(bullet["img"].value<QPointer<QLabel>>());
		bulletLabel->setProperty("currentTime", newCurrentTime);
		bulletLabel->move(ui->widgetLawnArea->x() + bullet["pos"].toPointF().x() * cellSize.width(),
						  ui->widgetLawnArea->y() + bullet["pos"].toPointF().y() * cellSize.height() - 7);
	}
	QList<QLabel *> bulletLabels = findChildren<QLabel *>("bullet", Qt::FindDirectChildrenOnly);
	for (QLabel *x : bulletLabels)
		if (x->property("currentTime") != newCurrentTime)
			x->deleteLater();
	mGameStatus->setProperty("bullets", bullets);

	QList<QVariant> sunshineList = mGameStatus->property("sunshineList").toList();
	for (int i = 0; i < sunshineList.length(); i++)
	{
		QMap<QString, QVariant> sunshine = sunshineList[i].toMap();
		QLabel *sunshineLabel;
		if (sunshine["img"].isNull())
		{
			sunshineLabel = createDynamicImage(":/interface/images/interface/Sun.gif", this);
			sunshineLabel->setObjectName("sunshine");
			sunshine["img"] = QVariant::fromValue(QPointer<QLabel>(sunshineLabel));
			sunshineLabel->resize(79, 79);
			QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
			sunshineLabel->setGraphicsEffect(opacityEffect);
			QPropertyAnimation *ani = new QPropertyAnimation(opacityEffect, "opacity", sunshineLabel);
			ani->setDuration(500);
			ani->setStartValue(0.);
			ani->setEndValue(.9);
			ani->start();
			sunshineLabel->show();
			sunshineList[i] = sunshine;
		}
		else
			sunshineLabel = (QLabel *)(sunshine["img"].value<QPointer<QLabel>>());
		sunshineLabel->move(ui->widgetLawnArea->x() + sunshine["pos"].toPointF().x() * cellSize.width(),
						  ui->widgetLawnArea->y() + sunshine["pos"].toPointF().y() * cellSize.height());
	}
	mGameStatus->setProperty("sunshineList", sunshineList);

	QMap<QString, QVariant> lastPlantTimeList = mGameStatus->property("lastPlantTime").toMap();
	for (int i = 1; i <= 5; i++)
	{
		QVariant lastPlantTime = lastPlantTimeList[GetPlantClassByID(i)->className()];
		Plant *plant = dynamic_cast<Plant *>(GetPlantClassByID(i)->newInstance());
		if (!lastPlantTime.isNull() && newCurrentTime - lastPlantTime.toLongLong() < plant->cd())
			findChild<PlantCard *>("widgetPlantCard" + QString::number(i))->setCoolDown(double(newCurrentTime - lastPlantTime.toLongLong()) / plant->cd());
		else if (plant->cost() > mGameStatus->property("sunvalue").toInt() ||
				 property("selectedPlant").toInt() == i)
			findChild<PlantCard *>("widgetPlantCard" + QString::number(i))->setCoolDown(0.);
		else
			findChild<PlantCard *>("widgetPlantCard" + QString::number(i))->setCoolDown(1.);
		delete plant;
	}
}

void PlayingInterface::timerEvent(QTimerEvent *)
{
	if (!mGameStatus->property("gameStartTime").isNull())
	{
		QElapsedTimer elapsedTimer;
		qint64 oldCurrentTime = mGameStatus->property("currentTime").toLongLong();
		mGameStatus->setProperty("lastFrameTime", oldCurrentTime);
		elapsedTimer.start();
		qint64 newCurrentTime = elapsedTimer.msecsSinceReference() - mGameStatus->property("gameStartTime").toLongLong();
		mGameStatus->setProperty("currentTime", newCurrentTime);
		mGameLogic->onTimeout(mGameStatus);
		for (const QVariant &x : mGameStatus->property("plants").toList())
			(x.value<QPointer<Plant>>())->onTimeout(mGameStatus);
		for (const QVariant &x : mGameStatus->property("zombies").toList())
			(x.value<QPointer<Zombie>>())->onTimeout(mGameStatus);
	}
	update();
}

void PlayingInterface::mousePressEvent(QMouseEvent *ev)
{
	QObject *clickedObject = childAt(ev->pos());
	if (clickedObject == nullptr)
	{
		QWidget::mouseMoveEvent(ev);
		return;
	}

	if (clickedObject->objectName() == "labelShovelBack")
	{
		if (property("shovelSelected").toBool())
		{
			setProperty("shovelSelected", QVariant());
			ui->labelShovel->show();
			setCursor(Qt::ArrowCursor);
		}
		else
		{
			setProperty("shovelSelected", true);
			setProperty("selectedPlant", QVariant());
			ui->labelShovel->hide();
			setCursor(QCursor(QPixmap(":/interface/images/interface/Shovel.png"), 1, 51));
		}
		return;
	}

	if (clickedObject->objectName() == "sunshine")
	{
		QList<QVariant> sunshineList = mGameStatus->property("sunshineList").toList();
		for (int i = 0; i < sunshineList.length(); i++)
		{
			QMap<QString, QVariant> sunshine = sunshineList[i].toMap();
			QLabel *sunshineLabel;
			if (sunshine["img"].isNull())
				continue;
			sunshineLabel = (QLabel *)(sunshine["img"].value<QPointer<QLabel>>());
			if (sunshineLabel == clickedObject)
			{
				mGameStatus->setProperty("sunvalue", mGameStatus->property("sunvalue").toInt() + sunshine["value"].toInt());
				sunshineList.removeAt(i);
				QPropertyAnimation *ani = new QPropertyAnimation(sunshineLabel, "pos");
				ani->setDuration(500);
				ani->setEasingCurve(QEasingCurve::OutCubic);
				ani->setStartValue(sunshineLabel->pos());
				ani->setEndValue(QPoint(0, 0));
				sunshineLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
				connect(ani, SIGNAL(finished()), sunshineLabel, SLOT(deleteLater()));
				ani->start();
				mGameStatus->setProperty("sunshineList", sunshineList);
				return;
			}
		}
		return;
	}

	QObject *p = clickedObject;
	do {
		if (p->objectName().startsWith("widgetPlantCard"))
		{
			if (mGameStatus->property("gameStartTime").isNull())
				return;
			int clickedPlantIndex = p->objectName().mid(15).toInt();
			if (!property("selectedPlant").isNull() && property("selectedPlant").toInt() == clickedPlantIndex)
				setProperty("selectedPlant", QVariant());
			else
			{
				QPointer<Plant> newPlant = dynamic_cast<Plant *>(GetPlantClassByID(clickedPlantIndex)->newInstance());
				newPlant->setPos(QPointF(-1., -1.));
				if (!newPlant->canPlant(mGameStatus))
				{
					newPlant->deleteLater();
					return;
				}
				if (property("shovelSelected").toBool())
				{
					setProperty("shovelSelected", QVariant());
					ui->labelShovel->show();
					setCursor(Qt::ArrowCursor);
				}
				setProperty("selectedPlant", clickedPlantIndex);
			}
			return;
		}
		else if (p->objectName() == "widgetLawnArea")
		{
			QSize mapSize = mGameStatus->property("mapSize").toSize();
			QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());
			QPoint relativePos = ui->widgetLawnArea->mapFrom(this, ev->pos());
			if (property("shovelSelected").toBool())
			{
				setProperty("shovelSelected", QVariant());
				ui->labelShovel->show();
				setCursor(Qt::ArrowCursor);

				QPointF plantPos = QPoint(qMin(relativePos.x() / cellSize.width(), mapSize.width() - 1),
										 qMin(relativePos.y() / cellSize.height(), mapSize.height() - 1));
				for (const QVariant &x : mGameStatus->property("plants").toList())
				{
					Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
					if (plant->pos() == plantPos)
						plant->onRemoved(mGameStatus);
				}
				return;
			}

			if (!mGameStatus->property("gameStartTime").isNull() && !property("selectedPlant").isNull())
			{
				QPoint plantPos = QPoint(qMin(relativePos.x() / cellSize.width(), mapSize.width() - 1),
										 qMin(relativePos.y() / cellSize.height(), mapSize.height() - 1));

				QPointer<Plant> newPlant = dynamic_cast<Plant *>(GetPlantClassByID(property("selectedPlant").toInt())->newInstance());
				newPlant->setPos(QPointF(plantPos));
				if (!newPlant->canPlant(mGameStatus))
				{
					newPlant->deleteLater();
					return;
				}
				newPlant->onPlanted(mGameStatus);

				QList<QVariant> plantsData(mGameStatus->property("plants").toList());
				plantsData.append(QVariant::fromValue(newPlant));
				mGameStatus->setProperty("plants", plantsData);

				setProperty("selectedPlant", QVariant());
				return;
			}
		}
		p = p->parent();
	} while (p != this);
}

void PlayingInterface::mouseMoveEvent(QMouseEvent *ev)
{
	Q_UNUSED(ev)
}

void PlayingInterface::onCreatureDestroyed(QObject *creature)
{
	if (creature != nullptr && !creature->property("img").isNull())
		(creature->property("img").value<QPointer<QLabel>>())->deleteLater();
	else
		qDebug() << "Null Pointer!";
}

void PlayingInterface::on_buttonMenu_clicked()
{
	qDebug() << "menu?" << endl;
}

void PlayingInterface::onGameFinished()
{
	killTimer(timerId);
	timerId = 0;
	if (mGameStatus->property("winner").toString() == "plants")
	{
		ui->labelPlantWin->raise();
		ui->labelPlantWin->show();
	}
	else
	{
		ui->labelZombieWin->raise();
		ui->labelZombieWin->show();
	}
}

QPointF PlayingInterface::screenToLawn(QPoint point)
{
	QSize mapSize = mGameStatus->property("mapSize").toSize();
	QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());
	QPointF relativePos = ui->widgetLawnArea->mapFrom(this, point);
	return QPointF(0., 0.);
}
