#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

class Client : public QObject
{
	Q_OBJECT

public:
	static Client *ins();

	QTcpSocket *socket;

private:
	Client() { }
	static Client *m_instance;
};

#endif // CLIENT_H
