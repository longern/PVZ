#ifndef REPLAYINTERFACE_H
#define REPLAYINTERFACE_H

#include "playinginterface.h"

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
	void keyPressEvent(QKeyEvent *);

public slots:

private:
	PlayingInterface *playInterface;

	int timerId;
	double replaySpeed;
	qint32 lastFrameTime;
	QFile *mGameRecordFile;
	QDataStream mGameRecordStream;
};

#endif // REPLAYINTERFACE_H
