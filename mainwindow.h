#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>

class MainWindow : public QWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	enum GameState { Welcome, Playing, GameOver };

public slots:
	void switchState(MainWindow::GameState state);

private:
	GameState mGameState;
	QWidget *mCurrentStateWidget;
};

#endif // MAINWINDOW_H
