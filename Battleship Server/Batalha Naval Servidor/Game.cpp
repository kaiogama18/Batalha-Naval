#include "Game.h"
#include <thread>

// Função responsavel para enviar as coordenadas para o cliente inimigo
const void Game::Attack(SOCKET* Connections, std::vector<std::string>& nicknames, int id_one, int id_two, Packet* packettype)
{
	SecureZeroMemory(m_Buffer, sizeof(m_Buffer));
	SecureZeroMemory(m_IsHit, sizeof(m_IsHit));
	memset(m_IsHit, '-', 1);
	// TAG [P_Ataque] Informa ao jogador atacante fazer uma jogada
	std::cout << "\n Vez do jogodor(a) [" << nicknames[id_one] << "]";

	*packettype = P_Ataque;

	m_Message = "-----> Informe as Coordenadas para o ataque " + nicknames[id_one] + +": ";
	send(Connections[id_one], (char*)packettype, sizeof(Packet), NULL);
	send(Connections[id_one], m_Message.c_str(), strlen(m_Message.c_str()), 0);

	// Recebendo as coordenadas
	recv(Connections[id_one], m_Buffer, sizeof(m_Buffer), 0);

	std::cout << "\n[" << nicknames[id_one]
		<< "] jogo nas coordenadas:  [" << m_Buffer << "]\n";

	// Tratamento das Coordenadas 
	std::string m_Coordinates = m_Buffer;

	// TAG [P_Coordenadas] Envia as coordenadas para o outro jogador 
	std::cout << " Coordenadas enviada para  jogador(a) "
		<< nicknames[id_two] << ": " << m_Buffer << '\n';

	*packettype = P_Coordenadas;

	send(Connections[id_two], (char*)packettype, sizeof(Packet), NULL);
	send(Connections[id_two], m_Buffer, sizeof(m_Buffer), 0);

	//  O retorno se acertou o navio ou não [O - X]
	SecureZeroMemory(m_IsHit, sizeof(m_IsHit));
	recv(Connections[id_two], m_IsHit, sizeof(m_IsHit), 0);
	std::string s_IsHit = m_IsHit;

	if (*m_IsHit == 'O')
		std::cout << "\n Foi Hit \n ";
	if (*m_IsHit == 'X')
		std::cout << "\n Não acertou \n ";

	*packettype = P_Update;
	send(Connections[id_one], (char*)packettype, sizeof(Packet), NULL);

	std::string m_update;
	m_update = m_Coordinates + m_IsHit;
	m_update.erase(std::remove_if(m_update.begin(), m_update.end(), isspace), m_update.end());

	send(Connections[id_one], m_update.c_str(), strlen(m_update.c_str()), 0);

	m_Temp = id_two;

	SecureZeroMemory(m_IsHit, sizeof(m_IsHit));
	SecureZeroMemory(m_Buffer, sizeof(m_Buffer));

	m_Coordinates.clear();
}


Game::Game(SOCKET* Connections, int counter, std::vector<std::string>& nicknames, Packet* packettype) : m_Temp{}
{
	// Enviando uma mensagem de boa vinda para todos os jogadores
	*packettype = P_Game;
	m_Message = "COMEÇOU O JOGO [" + nicknames[0] + " vs " + nicknames[1] + "]";

	// TAG: Mostrar a tabela para todos os jogadores
	for (int i = 0; i < counter; i++)
	{
		if (Connections[i] == 0)
		{
			std::cout << "Error #2\n";
		}
		else if (Connections[i] == INVALID_SOCKET)
		{
			std::cout << "Error: " << WSAGetLastError() << '\n';
			closesocket(Connections[i]);
		}
		else
		{
			send(Connections[i], (char*)packettype, sizeof(Packet), NULL);
			send(Connections[i], m_Message.c_str(), strlen(m_Message.c_str()), 0);
		}
	}

	Attack(Connections, nicknames, 0, 1, packettype);
	*packettype = P_Default;

	for (size_t i = 0; i < 100; i++)
	{
		if (Connections[i] == 0)
		{
			std::cout << "\n [Partida finalizada]\n";
			closesocket(Connections[i]);
			break;
		}
		else if (Connections[i] == INVALID_SOCKET)
		{
			std::cout << "\n [Partida finalizada]\n";
			closesocket(Connections[i]);
			break;

		}
		else
		{
			if (m_Temp == 0)
				Attack(Connections, nicknames, 0, 1, packettype);
			if (m_Temp == 1)
				Attack(Connections, nicknames, 1, 0, packettype);
		}
	}
}