#include "ui_welcomeform.h"
#include "welcomeinterface.h"
#include "sound.h"
#include <QInputDialog>

WelcomeInterface::WelcomeInterface(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::WelcomeForm)
{
	ui->setupUi(this);
	ui->buttonBeginAdventure->setMask(QPixmap(QStringLiteral(":/interface/images/interface/SelectorScreenStartAdventure_32.png")).mask());
	ui->buttonBeginSurvival->setMask(QPixmap(QStringLiteral(":/interface/images/interface/SelectorScreenSurvival_32.png")).mask());
	ui->buttonBeginChallenge->setMask(QPixmap(QStringLiteral(":/interface/images/interface/SelectorScreenChallenges_32.png")).mask());
	ui->buttonChangeUser->setMask(QPixmap(QStringLiteral(":/interface/images/interface/SelectorScreen_WoodSign2_32.png")).mask());
	Sound::ins()->play("qrc:/music/audio/Faster.mp3");
}

void WelcomeInterface::on_buttonBeginAdventure_clicked()
{
	emit stateSet(MainWindow::Playing);
}

void WelcomeInterface::on_buttonChangeUser_clicked()
{
	QString username = QInputDialog::getText(this, tr("Plants vs. Zombies"), tr("New user name:"));
	if(!username.isEmpty())
		ui->labelUserName->setText(username);
}

void WelcomeInterface::on_buttonBeginSurvival_clicked()
{
	emit stateSet(MainWindow::LastStand);
}
