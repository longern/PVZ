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
