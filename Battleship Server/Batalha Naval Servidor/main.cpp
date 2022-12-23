#include <iostream>
#include <ws2tcpip.h>
#include <vector>
#include <string>

#include <winsock2.h>
#include "stdafx.h"
#include <tchar.h>
#include <thread>

#include "Game.h"

SOCKET						Connections[100];			// Definição do número máximo de clientes
int							Counter = 0;				// Auxiliar para contar o número de clientes
fd_set						master;						// para colocar soquetes em um conjunto.
std::vector<std::string>	g_Nicknames;				// Guarda o nome do usuario

// Thread para tratar o Cliente //
void ConnectionHandler(int index)
{
	char nickname[50];
	ZeroMemory(nickname, 50);
	recv(Connections[index], nickname, sizeof(nickname), 0);

	Packet packettype = P_Default;

	send(Connections[index], (char*)&packettype, sizeof(Packet), NULL);

	std::string message = "-------> Bem vindo ao Servidor Batalha Naval ";
	message = message + nickname;
	send(Connections[index], message.c_str(), strlen(message.c_str()), 0);
	std::cout << "\n-----> Jogador(a) [" << index << "]: [ " << nickname << " ] entrou no Servidor!" << '\n';

	g_Nicknames.push_back(nickname);

	if (index == 0)
	{
		//packettype = P_Default;
		message = "\n-------> Esperando Outro Jogador! \n";
		send(Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		send(Connections[index], message.c_str(), strlen(message.c_str()), 0);
	}
	else if (index == 1)
	{
		Game game(Connections, Counter, g_Nicknames, &packettype);
		//packettype = P_Default;

	}
	else if (index > 1)
	{
		//packettype = P_Default;
		message = "\t-------> Já tem uma partida acontecendo ";
		send(Connections[index], (char*)&packettype, sizeof(Packet), NULL);
		send(Connections[index], message.c_str(), strlen(message.c_str()), 0);
	}
}

// Para varias chamadas de clientes usando threads //
void AcceptHandler(SOCKET connection)
{
	while (true)
	{
		SOCKET newConnection;
		for (int i = 0; i < 100; i++)
		{
			newConnection = ::accept(connection, nullptr, nullptr);

			if (newConnection == 0)
			{
				std::cout << "Error #2\n";
			}
			else if (newConnection == INVALID_SOCKET)
			{
				std::cout << "Error: " << WSAGetLastError() << '\n';
				closesocket(newConnection);
			}
			else
			{
				Connections[i] = newConnection;
				Counter++;
				std::thread connectionHandler = std::thread(ConnectionHandler, i);
				connectionHandler.detach();
				FD_SET(newConnection, &master);

			}
		}
	}
}

void StartServer()
{
	WSADATA version;
	// Função para inicialização para o windows
	if (WSAStartup(MAKEWORD(2, 2), &version) != 0)
	{
		std::cout << "WSADATA ERROR : Error no winsock. " << '\n';
		return;
	}
	std::cout << " WSAStartup OK!\n";

	SOCKET servidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servidor == INVALID_SOCKET)
	{
		std::cout << " Error: Falhou na criação do Socket ! \n";
		return;
	}
	std::cout << " Socket criado com sucesso! \n";

	// Criação da estrutura do endereço //
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(1111); // Qual a porta que o servidor vai usar


	// Conexão com servidor //
	auto result = bind(servidor, (sockaddr*)&service, sizeof(service));
	if (result == SOCKET_ERROR)
	{
		std::cout << " Error: Listen falhou \n";
		closesocket(servidor);
		WSACleanup();
		return;
	}

	// listen : Permite ficar ouvindo se alguem solicitar conexão
	result = listen(servidor, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		std::cout << " Error: Listen falhou \n";
		closesocket(servidor);
		WSACleanup();
		return;
	}
	std::cout << " Esperando Jogador! \n";

	// Thread para aceitar cliente
	std::thread acceptThread = std::thread(AcceptHandler, servidor);
	acceptThread.join();
	closesocket(servidor);
	WSACleanup();
}

int main()
{
	setlocale(LC_ALL, "portuguese");
	setlocale(LC_ALL, "en_US");
	std::cout << "----- Servidor: Batalha Naval -----\n\n";
	StartServer();
	system("PAUSE");
	return 0;
}