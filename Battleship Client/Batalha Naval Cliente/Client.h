#pragma once

#include"Player.h"



class Client
{
public:

	Client();

	const void		StartClient();
	const void		Menu();


	const void		SetBoardStatus(bool);
	const bool		GetBoardStatus();

private:

	Player*		m_Player;
	bool		m_BoardStatus;
};

enum Packet
{
	P_Coordenadas,
	P_Ataque,
	P_Game,
	P_Update,
	P_Default
};