//--------------------------------------------------------------------------------
//  Code		: Depending on the incoming message, the server automatically send
//				  a response to the program
//  Subject		: Computer Network
//  Professor	: Sang Bang Choi
//  File		: Server.cpp
//--------------------------------------------------------------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <stdio.h>			// Set to use printf() Function
#include <stdlib.h>			// Set to use exit() Function
#include <Winsock2.h>		// Set to use a Socket (Using winsock.h or winsoch2.h)
#pragma comment (lib, "ws2_32.lib")
// #include <arpa/inet.h>

#define MAX_SIZE 64         // Maximum Size of data that send at once (Byte).

int main() {

	// declare Winsock_data(WSADATA type structure)
	WSADATA winsock_data;
	// declare socket
	SOCKET Server_Socket;
	// declare structure of socket address information.
	SOCKADDR_IN Server_Address;
	// Set a connection port number. Random number between 2000 and 65535
	unsigned short Server_Port = 6801;

	// Before Windows socket start, DLL files associated with the socket must be initialized.
	if (WSAStartup(0x202, &winsock_data) == SOCKET_ERROR) {
		printf("WSAStartup Setting Error.\n");
		// Close initialization failure.
		WSACleanup();
		exit(0);
	}

	// Using AF_INET. Because we use TCP.
	Server_Address.sin_family = AF_INET;

	// IP address and connect port number
	// Test IP address : 127.0.0.1 (My computer internal IP address)
	Server_Address.sin_addr.s_addr = inet_addr("165.246.243.95");	// IP address
	Server_Address.sin_port = htons(Server_Port);				// Port Number

	// Create socket.
	Server_Socket = socket(AF_INET, SOCK_STREAM, 0);

	// Create socket error.
	if (Server_Socket == INVALID_SOCKET) {
		printf("Cannot create socket.");
		closesocket(Server_Socket);
		WSACleanup();
		exit(0);
	}

	if (bind(Server_Socket, (struct sockaddr*)&Server_Address, sizeof(Server_Address)) == SOCKET_ERROR) {
		printf("Cannot Bind.");
		closesocket(Server_Socket);
		WSACleanup();
		exit(0);
	}
	int retval;

	// Client Socket declaration.
	SOCKET Client_Socket;
	// Socket Address Structure declaration
	SOCKADDR_IN Client_Address;
	// Address size
	int AddressSize = sizeof(Client_Address);

	printf(" >>  Server starting...  << \n\n");
	int count = 1;

	// printf(123455)
		// Loop
		while (1) {

			if (listen(Server_Socket, SOMAXCONN) == SOCKET_ERROR) {
				printf("listen Error.\n");
				closesocket(Server_Socket);
				WSACleanup();
				exit(0);
			}

			// Client Socket declaration.
			//SOCKET Client_Socket;
			// Socket Address Structure declaration
			//SOCKADDR_IN Client_Address;
			// Address size
			//int AddressSize = sizeof(Client_Address);

			printf(" --------------------------------------------------------\n");
			// Connection Counting
			printf(" => Wait for Client(%d times).\n", count++);

			// compare to Client Data and Server Data.
			if ((Client_Socket = accept(Server_Socket, (struct sockaddr*)&Client_Address, &AddressSize)) == INVALID_SOCKET) {
				printf("Accept Error.\n");
				getchar();
			}
			else {
				printf(" => Connect IP: %s, Port : %d  Connect complete!\n\n", inet_ntoa(Client_Address.sin_addr), htons(Client_Address.sin_port));
			}

			//int retval;
			// Set the Buffer size larger than SendMessage[MAX_SIZE]
			char Buffer[999];
			retval = recv(Client_Socket, Buffer, sizeof Buffer, 0);

			printf(" => Message From Client : %s\n\n\n", Buffer);


			char Tmp_0[MAX_SIZE] = "Connect", Tmp_1[MAX_SIZE] = "Get", Tmp_2[MAX_SIZE] = "Put", Tmp_3[MAX_SIZE] = "Hi", Tmp_4[MAX_SIZE] = "Off"; // 여기 부분 수정하기

			int SendSize = MAX_SIZE;
			int ActualSendSize;

			// Sends a response to Client message.
			// If message is Unknown Command, Send a error message.
			// 채팅 메세지 추가할 것
			if (strcmp(Buffer, Tmp_0) == 0) { // if "Connect"
				char SendMessage[MAX_SIZE] = "Connection Open!";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}
			if (strcmp(Buffer, Tmp_1) == 0) { // if "Get"
				char SendMessage[MAX_SIZE] = "Download Files!";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}
			if (strcmp(Buffer, Tmp_2) == 0) { // if "Put"
				char SendMessage[MAX_SIZE] = "Upload Files!";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}
			if (strcmp(Buffer, Tmp_3) == 0) { // if "Hi"
				char SendMessage[MAX_SIZE] = "Hi. How are you doing?";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}
			if (strcmp(Buffer, Tmp_4) == 0) { // if "Off"
				char SendMessage[MAX_SIZE] = "Connection Close!";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}
			else { // if receive massage is not "Connect,Get,Put,Hi,Off"
				char SendMessage[MAX_SIZE] = "Unknown Command.";
				ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
			}

		}

	// Wait : Server state is listen.
	getchar();
}
