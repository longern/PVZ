#include "mainwindow.h"
#include "welcomeinterface.h"
#include "playinginterface.h"
#include "gameoverinterface.h"

MainWindow::MainWindow(QWidget *parent) :
	QWidget(parent)
{
	mCurrentStateWidget = nullptr;
	switchState(Welcome);
	setWindowTitle(tr("Plants vs. Zombies"));
	setWindowIcon(QIcon(QStringLiteral(":/interface/images/interface/SmallLogo.png")));
	setFixedSize(900, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::switchState(MainWindow::GameState state)
{
	if (mCurrentStateWidget == nullptr || mGameState != state)
	{
		mGameState = state;
		delete mCurrentStateWidget;
		switch (mGameState)
		{
		case Welcome:
			mCurrentStateWidget = new WelcomeInterface(this);
			break;
		case Playing:
			mCurrentStateWidget = new PlayingInterface(this);
			break;
		case GameOver:
			mCurrentStateWidget = new GameOverInterface(this);
			break;
		default:
			break;
		}
		mCurrentStateWidget->resize(900, 600);
		connect(mCurrentStateWidget, SIGNAL(stateSet(MainWindow::GameState)), this, SLOT(switchState(MainWindow::GameState)));
		mCurrentStateWidget->show();
	}
}
