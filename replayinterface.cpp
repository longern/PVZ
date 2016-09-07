#include "replayinterface.h"
#include "playinginterface.h"
#include "gamelogic.h"
#include "plants/plant.h"

ReplayInterface::ReplayInterface(QWidget *parent) : QWidget(parent)
{
	mGameRecordFile = nullptr;
}

ReplayInterface::~ReplayInterface()
{

}

void ReplayInterface::setReplayFile(const QString &fileName)
{
	playInterface = new PlayingInterface(this);
	playInterface->gameStatus()->setProperty("mode", "replay");
	connect(playInterface, SIGNAL(stateSet(MainWindow::GameState)), this, SIGNAL(stateSet(MainWindow::GameState)));
	playInterface->show();
	mGameRecordFile = new QFile(fileName);
	if (!mGameRecordFile->open(QIODevice::ReadOnly))
	{
		emit stateSet(MainWindow::Welcome);
		return;
	}

	mGameRecordStream.setDevice(mGameRecordFile);
	lastFrameTime = 0;
	timerId = startTimer(0);
}

void ReplayInterface::timerEvent(QTimerEvent *)
{
	if (playInterface->gameStatus()->property("gameStartTime").isNull())
		return;
	while (!mGameRecordStream.atEnd())
	{
		qint8 operationType;
		qint32 type, index;
		qint64 newCurrentTime;
		double x;
		QPointF pos;
		mGameRecordStream >> newCurrentTime >> operationType;
		playInterface->gameStatus()->setProperty("currentTime", newCurrentTime);
		switch (operationType)
		{
		case 'C':
			mGameRecordStream >> index;
			playInterface->gameLogic()->collectSunshine(playInterface->gameStatus(), index);
			break;
		case 'D':
			mGameRecordStream >> pos;
			for (const QVariant &x : playInterface->gameStatus()->property("plants").toList())
				if (x.value<QPointer<Plant>>()->pos() == pos)
					x.value<QPointer<Plant>>()->onRemoved(playInterface->gameStatus());
			break;
		case 'E':
			killTimer(timerId);
			break;
		case 'P':
			mGameRecordStream >> type >> pos;
			playInterface->gameLogic()->createPlant(playInterface->gameStatus(), type, pos.x(), pos.y());
			break;
		case 'S':
			mGameRecordStream >> x;
			playInterface->gameLogic()->createSunshine(playInterface->gameStatus(), x);
			break;
		case 'U':
			playInterface->gameStatus()->setProperty("lastFrameTime", lastFrameTime);
			playInterface->runGameLogic();
			lastFrameTime = newCurrentTime;
			update();
			return;
		case 'Z':
			mGameRecordStream >> type >> pos;
			playInterface->gameLogic()->createZombie(playInterface->gameStatus(), type, pos.x(), pos.y());
			break;
		default:
			break;
		}
	}
}
