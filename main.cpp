#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<WinSock2.h>
#include<iostream>
#include <string>

using namespace std;


#pragma comment(lib, "ws2_32.lib")

struct NumberPacket {
	int Number1;
	int Number2;
};

int main()
{
	WSAData wsaData;

	//Winsock �ʱ�ȭ 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Winsock Error : " << GetLastError() << endl;
		exit(-1);
	}

	//2. create socket
	SOCKET ServerSocket;

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "socket Error : " << GetLastError() << endl;
		exit(-1);
	}

	//3. ip, port�� �Ҵ��Ͽ� �����̶� ����
	SOCKADDR_IN ServerAddr;

	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(50000);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(ServerSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "bind Error : " << GetLastError() << endl;
		exit(-1);
	}

	//4. Ŭ���̾�Ʈ ���� ���
	if (listen(ServerSocket, 0) == SOCKET_ERROR)
	{
		cout << "listen Error : " << GetLastError() << endl;
		exit(-1);
	}

	while (1)
	{
		//5. Ŭ���̾�Ʈ�� ����
		SOCKADDR_IN ClientAddr;
		int ClinetAddrSize = sizeof(ClientAddr);
		SOCKET ClientSocket;
		ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddr, &ClinetAddrSize);
		if (ClientSocket == SOCKET_ERROR)
		{
			cout << "accept Error : " << GetLastError() << endl;
			exit(-1);
		}
		//inet network to address
		cout << "connect ip : " << inet_ntoa(ClientAddr.sin_addr) << endl;
		//network to host short
		cout << "connect port : " << ntohs(ClientAddr.sin_port) << endl;

		//6 �ڷḦ Ŭ���̾�Ʈ���� ����
		char Buffer[1024];
		recv(ClientSocket, Buffer, 1024, 0);

		string Numbers = Buffer;
		string Number1 = Numbers.substr(0, 2);
		string Number2 = Numbers.substr(3, 2);

		cout << "client sended : " << Number1 << endl;
		cout << "client sended : " << Number2 << endl;

		int IntResult = stoi(Number1) + stoi(Number2);

		string Result = to_string(IntResult);

		send(ClientSocket, Result.c_str(), Result.length() + 1, 0);

		cout << "client send : " << Result << endl;

		//6. ���� ����
		closesocket(ClientSocket);
	}

	closesocket(ServerSocket);

	//7.Winsock ����
	WSACleanup();

	return 0;
}