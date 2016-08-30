#include "plantcard.h"
#include "ui_plantcard.h"

PlantCard::PlantCard(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PlantCard)
{
	ui->setupUi(this);
}

PlantCard::~PlantCard()
{
	delete ui;
}
