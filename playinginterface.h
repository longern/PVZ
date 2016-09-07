#ifndef PLAYINGINTERFACE_H
#define PLAYINGINTERFACE_H

#include <QWidget>
#include "mainwindow.h"

class GameLogic;
class Zombie;

namespace Ui {
class PlayForm;
}

class PlayingInterface : public QWidget
{
	Q_OBJECT

public:
	explicit PlayingInterface(QWidget *parent = 0);
	~PlayingInterface();

	QObject *gameStatus() { return mGameStatus; }
	GameLogic *gameLogic() { return mGameLogic; }
	void runGameLogic();
	Q_INVOKABLE QLabel *createDynamicImage(const QString &imgSrc, QWidget *parent = 0);

signals:
	void stateSet(MainWindow::GameState);

protected:
	void paintEvent(QPaintEvent *);
	void timerEvent(QTimerEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void keyPressEvent(QKeyEvent *);

private slots:
	void onAnimationFinished();
	void onZombieCreated(Zombie *);
	void onSunshineCreated();
	void onSunshineCollected(int index);
	void onCreatureDestroyed(QObject *);
	void on_buttonBack_clicked();
	void onGameFinished();

private:
	Ui::PlayForm *ui;

	int timerId;
	QObject *mGameStatus;
	GameLogic *mGameLogic;
	QBuffer mGameRecord;
	QDataStream mGameRecordStream;

	void registerInterpolator();
	QPoint zombieAnimationOffset(Zombie *zombie);
};

#endif // PLAYINGINTERFACE_H
