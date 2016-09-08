#include "mainwindow.h"
#include "welcomeinterface.h"
#include "playinginterface.h"
#include "replayinterface.h"
#include "laststandinterface.h"

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
		case Replay:
			mCurrentStateWidget = new ReplayInterface(this);
			break;
		case LastStand:
			mCurrentStateWidget = new LastStandInterface(this);
			break;
		default:
			break;
		}
		mCurrentStateWidget->resize(900, 600);
		connect(mCurrentStateWidget, SIGNAL(stateSet(MainWindow::GameState)), this, SLOT(switchState(MainWindow::GameState)));
		mCurrentStateWidget->show();
	}
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
	if (ev->key() == Qt::Key_O && ev->modifiers() == Qt::ControlModifier)
	{
		QString replayFileName = QFileDialog::getOpenFileName(0, tr("Open Replay File"), QString(), tr("PVZ Replay File (*.pzr)"), 0, QFileDialog::DontUseNativeDialog);
		if (QFile::exists(replayFileName))
		{
			switchState(Replay);
			qobject_cast<ReplayInterface *>(mCurrentStateWidget)->setReplayFile(replayFileName);
		}
	}
}
