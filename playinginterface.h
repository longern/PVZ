#ifndef PLAYINGINTERFACE_H
#define PLAYINGINTERFACE_H

#include <QWidget>
#include "mainwindow.h"

class Plant;

namespace Ui {
class PlayForm;
}

class PlayingInterface : public QWidget
{
	Q_OBJECT

public:
	explicit PlayingInterface(QWidget *parent = 0);

	Q_INVOKABLE QLabel *createDynamicImage(const QString &imgSrc, QWidget *parent = 0);

signals:
	void stateSet(MainWindow::GameState);

protected:
	void paintEvent(QPaintEvent *);
	void timerEvent(QTimerEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);

private slots:
	void onAnimationFinished();
	void on_buttonMenu_clicked();

private:
	Ui::PlayForm *ui;

	QObject *mGameStatus;

	void registerInterpolator();
};

#endif // PLAYINGINTERFACE_H
