#include "Player.h"

#include <string>

Player::Player()
	: m_Health{ 21 },
	m_TotalShip{},
	m_MyBoard{
			{' ','A','B','C','D','E','F','G','H','I','J'},
			{'1','-','-','-','-','-','-','-','-','-','-'},
			{'2','-','-','-','-','-','-','-','-','-','-'},
			{'3','-','-','-','-','-','-','-','-','-','-'},
			{'4','-','-','-','-','-','-','-','-','-','-'},
			{'5','-','-','-','-','-','-','-','-','-','-'},
			{'6','-','-','-','-','-','-','-','-','-','-'},
			{'7','-','-','-','-','-','-','-','-','-','-'},
			{'8','-','-','-','-','-','-','-','-','-','-'},
			{'9','-','-','-','-','-','-','-','-','-','-'},
			{'0','-','-','-','-','-','-','-','-','-','-'},

	},
	m_EnemyBoard{
			{' ','A','B','C','D','E','F','G','H','I','J'},
			{'1','-','-','-','-','-','-','-','-','-','-'},
			{'2','-','-','-','-','-','-','-','-','-','-'},
			{'3','-','-','-','-','-','-','-','-','-','-'},
			{'4','-','-','-','-','-','-','-','-','-','-'},
			{'5','-','-','-','-','-','-','-','-','-','-'},
			{'6','-','-','-','-','-','-','-','-','-','-'},
			{'7','-','-','-','-','-','-','-','-','-','-'},
			{'8','-','-','-','-','-','-','-','-','-','-'},
			{'9','-','-','-','-','-','-','-','-','-','-'},
			{'0','-','-','-','-','-','-','-','-','-','-'},

	}
{
	m_Ships.push_back(Ship(1, "Submarinos", 2, 3, '2'));
	m_Ships.push_back(Ship(2, "Contratorpedeiros", 3, 2, '3'));
	m_Ships.push_back(Ship(3, "Navio-Tanque", 4, 1, '4'));
	m_Ships.push_back(Ship(4, "Porta-Avião", 5, 1, '5'));


	for (auto i = m_Ships.begin(); i != m_Ships.end(); ++i)
	{
		SetTotalShip(GetTotalShip() + i->GetQuantity());
	}
}

const void	Player::PrintBoard()
{
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		std::cout << '\n';
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (j == 0)
				std::cout << ' ' << m_MyBoard[i][j] << ' ';
			else
				std::cout << m_MyBoard[i][j] << ' ';
		}
	}
}

const void	Player::GameScreen()
{

	std::cout << "\n-----> [Sua Vida atual: " << m_Health << "]\n";
	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		std::cout << '\n';
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (j == 0)
				std::cout << ' ' << m_MyBoard[i][j] << ' ';
			else
				std::cout << m_MyBoard[i][j] << ' ';
		}
	}

	std::cout << "\n       SUA FROTA \n";
	std::cout << "\n----------------------\n";

	for (int i = 0; i < MATRIX_SIZE; ++i)
	{
		std::cout << '\n';
		for (int j = 0; j < MATRIX_SIZE; ++j)
		{
			if (j == 0)
				std::cout << ' ' << m_EnemyBoard[i][j] << ' ';
			else
				std::cout << m_EnemyBoard[i][j] << ' ';
		}
	}
	std::cout << "\n     FROTA INIMIGA\n\n";
}

const bool Player::AddShip()
{
	int id_ship{};
	char aligned{};
	int reppet{};
	std::pair<int, int> m_Coordinates;


	// Interação com o usuario para coletar informações sobre qual navio inserir

	for (size_t aux = 1; aux <= 7; aux++)
	{

		std::cout << "\n Coloque seus " << GetTotalShip() << " navios em campo! \n";
		for (int i = 0; i < MATRIX_SIZE; ++i)
		{
			std::cout << '\n';
			for (int j = 0; j < MATRIX_SIZE; ++j)
			{
				if (i != 0 && j != 0)
				{
					if (i == 10) std::cout << '[' << i << ',' << j << " ]" << ' ';
					else std::cout << "[ " << i << "," << j << " ]" << ' ';
				}
				else std::cout << "    " << m_MyBoard[i][j] << "   ";
			}
		}

		std::cout << "\n\n Você AINDA possui:\n";
		for (auto i = m_Ships.begin(); i != m_Ships.end(); ++i)
		{
			if (i->GetQuantity() != 0)
			{
				std::cout << " [ ID: " << i->GetId() << " ]" << " ..............."
					<< " " << i->GetQuantity() << "x "
					<< i->GetName() << " (" << i->GetSpace()
					<< " quadrados)" << '\n';
			}
		}

		std::cout << "\n Deseja adicionar qual navio?" << '\n';
		std::cout << "\n Informe o ID do Navio: ";
		std::cin >> id_ship;

		if (id_ship > 4)
		{
			system("CLS");
			std::cout << "\n ID do navio: [" << id_ship << "] [ INVÁLIDO ] \n";
			AddShip();
		}

		std::cout << " Informe as COORDENADAS iniciais:  ";
		std::cin >> m_Coordinates.first;
		std::cin >> m_Coordinates.second;

		if (m_Coordinates.first > 10 || m_Coordinates.second > 10)
		{
			system("CLS");
			std::cout << "\n COORDENADA na posição: [" << m_Coordinates.first << "," << m_Coordinates.second << "] [ INVÁLIDO ] \n";
			AddShip();
		}

		std::cout << " Alinhar o navio na [ VERTICAL - HORIZONTAL ]: ";
		std::cin >> aligned;
		aligned = tolower(aligned);

		if (aligned != 'v' && aligned != 'h')
		{
			system("CLS");
			std::cout << "\n POSIÇÃO do navio: [" << aligned << "] [ INVÁLIDO ] \n";
			AddShip();
		}

		// Adicionando o navio na tabela 
		for (auto aux = m_Ships.begin(); aux != m_Ships.end(); ++aux)
		{
			if (id_ship == aux->GetId() && aux->GetQuantity() != 0)
			{
				std::cout << "\n Adicionando: " << aux->GetName() << " (" << aux->GetSpace() << " quadrados)";
				std::cout << "\n Na posição: [" << m_Coordinates.first << "," << m_Coordinates.second << "]\n";

				if (aligned == 'h' && m_Coordinates.second + (aux->GetSpace() - 1) < MATRIX_SIZE - 1)
				{
					for (int reppet = 0; reppet < aux->GetSpace(); ++reppet)
						m_MyBoard[m_Coordinates.first][m_Coordinates.second + reppet] = aux->GetIcon();
				}
				else if (aligned == 'v' && m_Coordinates.first + (aux->GetSpace() - 1) < MATRIX_SIZE - 1)
				{
					for (int reppet = 0; reppet < aux->GetSpace(); ++reppet)
						m_MyBoard[m_Coordinates.first + reppet][m_Coordinates.second] = aux->GetIcon();
				}
				aux->SetQuantity(aux->GetQuantity() - 1);
				m_Health = aux->GetQuantity();
				SetTotalShip(GetTotalShip() - 1);
				PrintBoard();
				std::cout << "\n\n " << aux->GetName() << " adicionado com sucesso! \n";
				system("PAUSE");
			}
		}
	}

	return true;
}

const int Player::GetHealth()
{
	return m_Health;
}

const void Player::SetHealth(int health)
{
	m_Health = health;
}

const void Player::SetTotalShip(int ship)
{
	m_TotalShip = ship;
}

const int Player::GetTotalShip()
{
	return m_TotalShip;
}

Player::~Player()
{
	delete m_MyBoard;
}

const std::string Player::IsHit(std::string m_coordinate)
{
	//system("CLS");

	std::pair<int, int> m_Coordinates;
	// Tratamento das coordenadas para int
	m_coordinate.erase(std::remove_if(m_coordinate.begin(), m_coordinate.end(), isspace), m_coordinate.end());
	m_Coordinates.first = (int)m_coordinate[0] - 48;
	m_Coordinates.second = (int)m_coordinate[1] - 48;

	std::string m_Hit;

	// Atualizando o tabuleiro inimigo
	if (m_coordinate.size() > 2 && m_coordinate.size() == 1)
	{
		std::cout << "\n COORDENADA [ INVÁLIDO ] \n";
	}
	else
	{
		if (m_MyBoard[m_Coordinates.first][m_Coordinates.second] != '-')
		{
			m_MyBoard[m_Coordinates.first][m_Coordinates.second] = 'X';
			m_Hit = 'O';
			SetHealth(GetHealth() - 1);

			GameScreen();

			std::cout << "\n-----> Você levou dano nesta Rodada na posiçao: ["
				<< m_Coordinates.first << "," << m_Coordinates.second << "]";
			std::cout << "\n-----> [Sua Vida atual: " << m_Health << "]\n";
		}
		else
		{
			m_Hit = 'X';
			std::cout << "\n-----> O tiro caiu no mar na posição: ["
				<< m_Coordinates.first << "," << m_Coordinates.second << "]";
			std::cout << "\n-----> [Sua Vida atual: " << m_Health << "]\n";
		}
	}
	return m_Hit;
}

const void Player::Update(std::string m_coordinate)
{
	std::pair<int, int> m_Coordinates;
	char m_Hit = m_coordinate[2];

	// Tratamento das coordenadas para int
	m_coordinate.erase(std::remove_if(m_coordinate.begin(), m_coordinate.end(), isspace), m_coordinate.end());
	m_Coordinates.first = (int)m_coordinate[0] - 48;
	m_Coordinates.second = (int)m_coordinate[1] - 48;

	m_EnemyBoard[m_Coordinates.first][m_Coordinates.second] = m_Hit;

	GameScreen();
	if (m_Hit == 'O')
	{
		std::cout << "\n-----> Você acertou na posição: ["
			<< m_Coordinates.first << "," << m_Coordinates.second << "]";
	}
	if (m_Hit == 'X')
	{
		std::cout << "\n-----> Você errou na posição: ["
			<< m_Coordinates.first << "," << m_Coordinates.second << "]";
	}
}
