

#include "stdafx.h"
#include <string>
#include <iostream>
#include "Winsock2.h" //заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") //экспорт WS2_32.dll
#include "Error.h"
#include <ctime>

#define SERVER_ADDRESS "192.168.43.181"

using namespace std;

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

char* get_message(int msg)
{
	char echo[] = "Echo";
	char time[] = "Time";
	char rand[] = "0001";
	char r[] = "";
	switch (msg)
	{
	case 1: 	return	echo;
	case 2: 	return  time;
	case 3: 	return  rand;
	default:
		return r;
	}
}

int main()
{
	SOCKET  cS;
	SOCKET  cC;
	WSADATA wsaData;
	setlocale(0, "rus");
	try
	{	
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());


		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

		HANDLE          hTimer;
		BOOL            bSuccess;
		__int64         qwDueTime;
		LARGE_INTEGER   liDueTime;


		if ((cC = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());
		if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw  SetErrorMsgText("connect:", WSAGetLastError());
		bool finish = false;


		while (!finish)
		{
			char message[50];
			int  libuf = 0, lobuf = 0, service;

			cout << "Choose\n1 - Echo\n2 - Time\n3 - Random\n4 - Exit\n";
			
			scanf("%d", &service);
			if (service == 4)
			{
				break;
			}
			char* outMessage = new char[5];
			strcpy(outMessage, get_message(service));

			if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("send:", WSAGetLastError());

			printf("send: %s\n", strcmp(outMessage,"0001") == 0 ? "Rand" : outMessage);

			if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());

			if (!strcmp(message, "TimeOUT")) {
				cout << "Time out\n";
				break;
			}

			if (service == 1)
			{
				for (int j = 10; j >= 0; --j) {
					Sleep(1000);
					sprintf(outMessage, "%d", j);
					if ((lobuf = send(cC, outMessage, strlen(outMessage) + 1, NULL)) == SOCKET_ERROR)
						throw  SetErrorMsgText("send:", WSAGetLastError());

					printf("send: %s\n", outMessage);

					if ((libuf = recv(cC, message, sizeof(message), NULL)) == SOCKET_ERROR)
						throw  SetErrorMsgText("recv:", WSAGetLastError());

					printf("receive: %s\n", message);				
				}

			}
			else if (service == 2 || service == 3) {
				printf("receive: %s\n", message);

			}
			else if (service == 4) {
				break;
			}
			else {

				printf("receive: %s\n", message);
			}
		}
		
		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;

		if (closesocket(cC) == SOCKET_ERROR)
			throw  SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	system("pause");
	return 0;
}

