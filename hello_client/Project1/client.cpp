//--------------------------------------------------------------------------------
//  Code		: Client to send a message to the server
//  Subject		: Computer Network
//  Professor	: Sang Bang Choi
//  File		: Client.cpp 
//--------------------------------------------------------------------------------

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <stdio.h>			// Set to use printf() Function
#include <stdlib.h>			// Set to use exit() Function
#include <Winsock2.h>		// Set to use a Socket (Using winsock.h or winsoch2.h)
#pragma comment (lib, "ws2_32.lib")

#define MAX_SIZE 64			// Maximum Size of data that send at once (Byte).

int main() {
	// declare Winsock_data(WSADATA type structure)
	
	WSADATA winsock_data;
	// declare socket
	SOCKET Client_Socket;
	// declare structure of socket address information.
	SOCKADDR_IN Server_Address;
	// Set a connection port number. Random number between 2000 and 65535
	unsigned short Server_Port = 6801; // port number 일치시키기
	int ReturnVal;
	// Create socket
	
	//Client_Socket = socket(AF_INET, SOCK_STREAM, 0);
	
	
	// Before Windows socket start, DLL files associated with the socket must be initialized.
	if (WSAStartup(0x202, &winsock_data) == SOCKET_ERROR) {
		printf("WSAStartup Setting Error.\n");
		// Close initialization failure.
		WSACleanup();
		exit(0);
	}

	int count = 1;	// Connection count
	int retval;
	// declare variable to send message, (Max.Size = MAX_SIZE)
	char SendMessage[MAX_SIZE];
	char Tmp[MAX_SIZE] = "Off"; // Compare to the Client end.

	// SendMessage[MAX_SIZE] is message that send from Server to Client.
	while (strcmp(SendMessage, Tmp) != 0) {
		Client_Socket = socket(AF_INET, SOCK_STREAM, 0);
		// Create socket
		//Client_Socket = socket(AF_INET, SOCK_STREAM, 0);

		// error to Create socket
		if (Client_Socket == INVALID_SOCKET) {
			printf("Create Socket Error.");
			WSACleanup();
			exit(0);
		}

		// Using AF_INET. Because we use TCP.
		Server_Address.sin_family = AF_INET;

		// IP address and connect port number
		// Test IP address : 127.0.0.1 (My computer internal IP address)
		Server_Address.sin_addr.s_addr = inet_addr("165.246.243.47");	// IP address
		Server_Address.sin_port = htons(Server_Port);				// Port Number

		// Attempts to connect to the server.
		ReturnVal = connect(Client_Socket, (struct sockaddr*)&Server_Address, sizeof(Server_Address));

		printf("\n --------------------------------------------------------\n");

		// if ReturnVal is 1, Connection setup error.
		if (ReturnVal) {
			printf(" >>  Server Connection Error.  << \n\n");
			closesocket(Client_Socket);
			WSACleanup();
			exit(0);
		}
		else {
			// Count the number of connections.
			printf(" >> connected to the server( %d times ).  << \n\n", count++);
		}

		int SendSize = MAX_SIZE;
		int ActualSendSize;

		// Command List.
		printf(" :: Command List : Connect , Get , Put , Hello Server , Off, Good bye (Case Sensitive)\n\n"); // 이쪽을 조절하기

		// Wait message that send to server.
		printf(" => Message input : ");
		gets_s(SendMessage);

		// Ready to send message 
		ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);

		// int retval;
		// Set the Buffer size larger than SendMessage[MAX_SIZE]
		char Buffer[999];
		// Receive data from socket and save buffer.
		retval = recv(Client_Socket, Buffer, sizeof Buffer, 0);

		// Display a message that receive data from server.
		printf(" => message from Server : %s\n\n", Buffer);

	}//while end.
	
	// If Server request "OFF" from Client, Close the socket to break the connection with the server.
	closesocket(Client_Socket);
	WSACleanup();
	printf("\n\n ※ Client Program Close.\n");
	// Wait
	getchar();
}