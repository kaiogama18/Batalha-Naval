#pragma once

#include <iostream>
#include <ws2tcpip.h>
#include <vector>
#include <string>

#define		BUFFER_SIZE 512

enum Packet
{
	P_Coordenadas,	
	P_Ataque,		
	P_Game,			
	P_Update,		
	P_Default		
};

class Game
{
public:

	Game(SOCKET*, int, std::vector<std::string>&, Packet*);
	const void Attack(SOCKET*, std::vector<std::string>&, int, int, Packet*);

private:
	
	std::string			m_Message;
	char				m_Buffer[BUFFER_SIZE];
	char				m_IsHit[BUFFER_SIZE];
	int					m_Temp;
	Packet				packettype;
};

