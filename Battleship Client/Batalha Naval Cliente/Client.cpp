
#include"Client.h"

#include <iostream>
#include <ws2tcpip.h>
#include <vector>
#include <string>

#include <winsock2.h>
#include "stdafx.h"
#include <tchar.h>
#include <thread>

#include "Player.h"

#define		PORT 1111				// Definição da porta padrão
#define		BUFFER_SIZE 512			// Definição do tamanho do buffer das mensagens
std::string g_name;					// Guarda o nome do jagador
char		recvbuf[BUFFER_SIZE];	// Variável que recebe a resposta do servidor
std::string data;


Client::Client() : m_BoardStatus{ false }
{
	//Um jogador para um cliente
	m_Player = new Player;

	//Vida do jogador é a somatoria de todos os quadrados dos barcos = 21 
	m_Player->SetHealth(21);
	Menu();
}

const void Client::Menu()
{
	int op;
	std::cout << "\n ----- [ MENU ]\n\n";
	std::cout << R"( 1 - Entrar no Servidor
 2 - Cadastra Navios
 3 - Imprimir o Tabuleiro
 0 - Sair )" << '\n';
	std::cout << "\n-----> Informe a opção: ";
	std::cin >> op;
	std::cin.clear();

	switch (op)
	{
	case 1:
		std::cin.ignore();
		if (GetBoardStatus() == true)
		{
			StartClient();
		}
		else
		{
			system("CLS");
			std::cout << "\n-----> [ SEM NAVIOS NO TABULEIRO! ]\n";
			SetBoardStatus(m_Player->AddShip());
			Menu();
		}
		break;
	case 2:
		SetBoardStatus(m_Player->AddShip());
		Menu();
		break;

	case 3:
		m_Player->PrintBoard();
		Menu();

		break;
	case 0:
		break;
	default:
		break;
	}
}

const bool Client::GetBoardStatus()
{
	return m_BoardStatus;
}

const void Client::SetBoardStatus(bool status)
{
	m_BoardStatus = status;
}

// [SendHandle] -- Enviar o conteudo de volta para o servidor 
void SendHandle(SOCKET Connection)
{
	do
	{
		if (data.size() > 0)
		{
			if (send(Connection, data.c_str(), strlen(data.c_str()), 0) == SOCKET_ERROR)
			{
				std::cout << "send failed: " << WSAGetLastError() << "\n";
				closesocket(Connection);
				WSACleanup();
				return;
			}
		}
	} while (data.size() > 0);
}

// Verificar qual das TAGs foi enviada pelo servidor
void ProcessPacket(Packet packettype, SOCKET Connection, Player* player, char buffer[512])
{
	switch (packettype)
	{
	case 0: //P_Coordenadas
	{
		////////// [0] = P_Coordenadas 
		std::string hit = player->IsHit(buffer);
		if (player->GetHealth() == 0)
		{
			std::cout << "\n ---------- [VOCÊ PERDEU] ----------\n";
			hit = "\n ---------- [VOCÊ GANHOU] ----------\n";
		}
		send(Connection, hit.c_str(), strlen(hit.c_str()), 0);
		std::cin.clear();
		break;
	}
	case 1:
	{
		////////// [1] = P_Ataque 
		std::cout << " [P_Ataque] " << buffer;
		// Recebe o valor do ataque do ursuario
		std::getline(std::cin, data);
		std::cin.clear();
		// Envia os dados de volta para o servidor
		send(Connection, data.c_str(), strlen(data.c_str()), 0);
		break;
	}
	case 2:
	{
		////////// [2] = P_Game
		player->GameScreen();
		break;
	}
	case 3:
	{
		////////// [3] = P_Update 
		std::cout << "\n " << buffer << '\n';
		player->Update(buffer);
		SecureZeroMemory(buffer, sizeof(buffer));
		if (player->GetHealth() == 0)
		
		break;
	}
	case 4:
		////////// [4] = P_Default
		std::cout << '\r';
		std::cout << buffer << "\n";
		break;
	default:
		std::cout << "\nTAG não reconhecido: " << packettype << std::endl;
		break;
	}
}

void RecvHandle(SOCKET Connection, Player* player)
{

	Packet packettype{};
	while (true)
	{
		char recvbuf[BUFFER_SIZE];
		ZeroMemory(recvbuf, BUFFER_SIZE);
		std::cin.clear();
		// Declaração: Receber os dados do servidor
		auto tag = recv(Connection, (char*)&packettype, sizeof(Packet), NULL);
		auto iRecv = recv(Connection, recvbuf, BUFFER_SIZE, 0);
		// Loop para ficar escuntando o servidor se o resultado de tag e iRev é maior do que 0
		if (iRecv > 0 && tag > 0)
		{
			ProcessPacket(packettype, Connection, player, recvbuf);
		}
		else if (iRecv == 0)
		{
			std::cout << "Connection closed" << "\n";
			return;
		}
		else
		{
			std::cout << "recv failed: " << WSAGetLastError() << "\n";
			closesocket(Connection);
			WSACleanup();
			std::getchar();
			return;
		}
	}
}

const void Client::StartClient()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "WSADATA ERROR : Error no winsock." << "\n";
		return;
	}
	SOCKET clientSoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSoc == INVALID_SOCKET)
	{
		std::cout << "Error: falha na conexão ( Falha na criação do Socket )" << "\n";
		closesocket(clientSoc);
		WSACleanup();
		return;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(1111); // Qual a porta que o servidor vai usar

	// Conexão com servidor //
	auto iConnect = connect(clientSoc, (SOCKADDR*)&service, sizeof(service));
	if (iConnect == SOCKET_ERROR)
	{
		std::cout << "Error: falha na conexão" << "\n";
		closesocket(clientSoc);
		WSACleanup();
		return;
	}
	std::cin.clear();

	std::cout << "\n\tInfome seu NickName: ";
	std::getline(std::cin, g_name);
	std::cout << "\n";

	auto servidor = send(clientSoc, g_name.c_str(), strlen(g_name.c_str()), 0);
	if (servidor == SOCKET_ERROR)
	{
		std::cout << "Sending Failed " << "\n";
		std::cout << "Error No-> " << WSAGetLastError() << "\n";
		closesocket(clientSoc);
		WSACleanup();
		return;
	}

	std::thread senderThread = std::thread(SendHandle, clientSoc);
	senderThread.detach();

	std::thread recverThread = std::thread(RecvHandle, clientSoc, m_Player);
	recverThread.join();

	closesocket(clientSoc);
	WSACleanup();
}
