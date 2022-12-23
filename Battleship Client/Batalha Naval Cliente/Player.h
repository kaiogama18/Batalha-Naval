#pragma once

#include<iostream>
#include <vector>
#include <string>

#include "Ship.h"

// Tamanho da matrix padrão
#define		MATRIX_SIZE 11 

class Player
{
public:

	Player();

	const void			PrintBoard();
	const void			GameScreen();

	const bool			AddShip();
	const void			SetHealth(int hit);
	const int			GetHealth();

	const void			SetTotalShip(int);
	const int			GetTotalShip();

	const std::string	IsHit(std::string);
	const void			Update(std::string);

	~Player();

private:

	int					m_Health;
	int					m_TotalShip;
	char				m_MyBoard[MATRIX_SIZE][MATRIX_SIZE];
	char				m_EnemyBoard[MATRIX_SIZE][MATRIX_SIZE];
	std::vector<Ship>	m_Ships;

};
