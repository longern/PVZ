#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <QObject>
#include <QPair>
#include <functional>

class GameLogic : public QObject
{
	Q_OBJECT
public:
	explicit GameLogic(QObject *parent = 0);

	Q_INVOKABLE void onGameStart(QObject *root);
	Q_INVOKABLE void onTimeout(QObject *root);
	Q_INVOKABLE void addTimeFlag(qint64 timePoint, std::function<void(QObject *)> func);

signals:
	void gameFinished();

private:
	QList<QPair<qint64, std::function<void (QObject *)>>> timeFlags;
};

#endif // GAMELOGIC_H
