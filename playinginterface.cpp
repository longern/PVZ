#include "playinginterface.h"
#include "ui_playform.h"
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
	QPainter painter;
}

void PlayingInterface::timerEvent(QTimerEvent *)
{
	for (const QVariant &x : mGameStatus->property("plants").toList())
		(x.value<QPointer<Plant>>())->onTimeout(mGameStatus);
	for (const QVariant &x : mGameStatus->property("zombies").toList())
		(x.value<QPointer<Plant>>())->onTimeout(mGameStatus);
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
			if (!property("gameStartTime").isNull() && !property("selectedPlant").isNull())
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

				QLabel *plantMovieLabel = createDynamicImage(newPlant->imgSrc(), ui->widgetLawnArea);
				plantMovieLabel->resize(cellSize);
				plantMovieLabel->move(plantPos.x() * cellSize.width(), plantPos.y() * cellSize.height());
				plantMovieLabel->show();
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
