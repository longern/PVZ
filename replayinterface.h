#ifndef REPLAYINTERFACE_H
#define REPLAYINTERFACE_H

#include "playinginterface.h"

class GameLogic;

class ReplayInterface : public QWidget
{
	Q_OBJECT

public:
	explicit ReplayInterface(QWidget *parent = 0);
	~ReplayInterface();
	void setReplayFile(const QString &fileName);

signals:
	void stateSet(MainWindow::GameState);

protected:
	void timerEvent(QTimerEvent *);

public slots:

private:
	PlayingInterface *playInterface;

	int timerId;
	qint64 lastFrameTime;
	QFile *mGameRecordFile;
	QDataStream mGameRecordStream;
};

#endif // REPLAYINTERFACE_H
