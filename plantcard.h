#ifndef PLANTCARD_H
#define PLANTCARD_H

#include <QWidget>

namespace Ui {
class PlantCard;
}

class PlantCard : public QWidget
{
	Q_OBJECT

public:
	explicit PlantCard(QWidget *parent = 0);
	~PlantCard();

private:
	Ui::PlantCard *ui;
};

#endif // PLANTCARD_H
