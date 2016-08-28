#include "client.h"

Client *Client::m_instance = nullptr;

Client *Client::ins()
{
	if (m_instance == nullptr)
		m_instance = new Client;
	return m_instance;
}
