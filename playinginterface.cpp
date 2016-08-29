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

	for (int i = 1; i <= 4; i++)
	{
		QLabel *card = ui->widgetCardArea->findChild<QLabel *>("labelPlantAvatar" + QString::number(i));
		const QMetaObject *plantClass = GetPlantClassByID(i);
		if (card && plantClass)
			card->setPixmap(QPixmap(plantClass->classInfo(plantClass->indexOfClassInfo("staticImageSource")).value()).scaledToHeight(31, Qt::SmoothTransformation));
	}

	mGameStatus = new QObject(this);
	mGameStatus->setProperty("mapSize", QSize(9, 5));
	mGameStatus->setProperty("sunshine", 50);

	registerInterpolator();
	onAnimationFinished();  // Activate first animation

	startTimer(0);
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
	QSize mapSize = mGameStatus->property("mapSize").toSize();
	QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());
	for (const QVariant &x : mGameStatus->property("plants").toList())
	{
		Plant *plant = (Plant *)(x.value<QPointer<Plant>>());
		if (plant->property("img").isNull())
		{
			QLabel *plantMovieLabel = createDynamicImage(plant->imgSrc(), ui->widgetLawnArea);
			plantMovieLabel->resize(cellSize);
			plantMovieLabel->move(plant->pos().x() * cellSize.width(),
								  plant->pos().y() * cellSize.height() - 10);
			plantMovieLabel->show();
			plant->setProperty("img", QVariant::fromValue(QPointer<QLabel>(plantMovieLabel)));
		}
	}
	for (const QVariant &x : mGameStatus->property("zombies").toList())
	{
		Zombie *zombie = (Zombie *)(x.value<QPointer<Zombie>>());
		QLabel *zombieMovieLabel;
		if (zombie->property("img").isNull())
		{
			zombieMovieLabel = createDynamicImage(zombie->imgSrc(), this);
			zombie->setProperty("img", QVariant::fromValue(QPointer<QLabel>(zombieMovieLabel)));
			zombieMovieLabel->resize(166, 144);
			zombieMovieLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
		}
		else
			zombieMovieLabel = (QLabel *)(zombie->property("img").value<QPointer<QLabel>>());
		zombieMovieLabel->move(ui->widgetLawnArea->x() + zombie->pos().x() * cellSize.width() - 60,
							   ui->widgetLawnArea->y() + zombie->pos().y() * cellSize.height() - 70);
		zombieMovieLabel->show();
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
		gameLogic->onTimeout(mGameStatus);
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

	QObject *p = clickedObject;
	do {
		if (p->objectName().startsWith("widgetPlantCard"))
		{
			int clickedPlantIndex = p->objectName().mid(15).toInt();
			if (!property("selectedPlant").isNull() && property("selectedPlant").toInt() == clickedPlantIndex)
				setProperty("selectedPlant", QVariant());
			else
				setProperty("selectedPlant", clickedPlantIndex);
			return;
		}
		else if (p->objectName() == "widgetLawnArea")
		{
			if (!mGameStatus->property("gameStartTime").isNull() && !property("selectedPlant").isNull())
			{
				QSize mapSize = mGameStatus->property("mapSize").toSize();
				QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());
				QPoint relativePos = ui->widgetLawnArea->mapFrom(this, ev->pos());
				QPoint plantPos = QPoint(qMin(relativePos.x() / cellSize.width(), mapSize.width() - 1),
										 qMin(relativePos.y() / cellSize.height(), mapSize.height() - 1));

				QPointer<Plant> newPlant = dynamic_cast<Plant *>(GetPlantClassByID(property("selectedPlant").toInt())->newInstance());
				newPlant->setPos(QPointF(plantPos));
				if (!newPlant->canPlant(mGameStatus))
				{
					newPlant->deleteLater();
					return;
				}

				QList<QVariant> plantsData(mGameStatus->property("plants").toList());
				plantsData.append(QVariant::fromValue(newPlant));
				mGameStatus->setProperty("plants", plantsData);

				setProperty("selectedPlant", QVariant());
			}
			return;
		}
		p = p->parent();
	} while (p != this);
}

void PlayingInterface::mouseMoveEvent(QMouseEvent *ev)
{
	Q_UNUSED(ev)
}

void PlayingInterface::on_buttonMenu_clicked()
{
	qDebug() << "menu?" << endl;
}

QPointF PlayingInterface::screenToLawn(QPoint point)
{
	QSize mapSize = mGameStatus->property("mapSize").toSize();
	QSize cellSize(ui->widgetLawnArea->width() / mapSize.width(), ui->widgetLawnArea->height() / mapSize.height());
	QPointF relativePos = ui->widgetLawnArea->mapFrom(this, point);
	return QPointF(0., 0.);
}
