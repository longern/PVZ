#include "plantcard.h"
#include "ui_plantcard.h"

PlantCard::PlantCard(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlantCard)
{
	ui->setupUi(this);
	ui->labelPlantBack->setAttribute(Qt::WA_TransparentForMouseEvents);
	ui->labelPlantAvatar->setAttribute(Qt::WA_TransparentForMouseEvents);
	ui->labelPlantCost->setAttribute(Qt::WA_TransparentForMouseEvents);
}

PlantCard::~PlantCard()
{
	delete ui;
}

void PlantCard::setPlantAvatar(const QString &avatar)
{
	ui->labelPlantAvatar->setPixmap(QPixmap(avatar).scaledToWidth(30, Qt::SmoothTransformation));
}

void PlantCard::setPlantCost(int cost)
{
	ui->labelPlantCost->setText(QString::number(cost));
}

void PlantCard::setCoolDown(double coolDown)
{
	if(coolDown == 1.)
	{
		ui->labelPlantMask->setStyleSheet("");
	}
	else
	{
		ui->labelPlantMask->setStyleSheet(QString("background-color:"
			"qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0,"
							 "stop:0 rgba(0, 0, 0, 127),"
							 "stop:%1 rgba(0, 0, 0, 127),"
							 "stop:%2 rgba(0, 0, 0, 191),"
							 "stop:1 rgba(0, 0, 0, 191));").arg(coolDown).arg(coolDown + 1e-3));
	}
}
