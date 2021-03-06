//--------------------------------------------------------------------------------
//  Code		: Depending on the incoming message, the server automatically send
//				  a response to the program
//  Subject		: Computer Network
//  Professor	: Sang Bang Choi
//  File		: Server.cpp
//--------------------------------------------------------------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include <typeinfo> 
#include <stdio.h>			// Set to use printf() Function
#include <stdlib.h>			// Set to use exit() Function
#include <Winsock2.h>		// Set to use a Socket (Using winsock.h or winsoch2.h)
#pragma comment (lib, "ws2_32.lib")
#include <stdio.h>
// #include <arpa/inet.h>

#define MAX_SIZE 64         // Maximum Size of data that send at once (Byte).
#define _CRT_SECURE_NO_WARNINGS

double calculator(double x1, double x2, char eq)
{

	double num1, num2;
	num1 = x1;
	num2 = x2;

	double result;

	switch (eq)
	{
	case '+':
		result = num1 + num2;
		break;

	case '-':
		result = num1 - num2;
		break;

	case '*':
		result = num1 * num2;
		break;

	case '/':
		if (num2 != 0) {
			result = num1 / num2;
			break;
		}
		else {
			break; //0으로 나누는 경우는 제외한다.
		}

	default:
		// If the operator is other than +, -, * or /, error message is shown

		break;
	}

	return result;
}
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
	// Test IP address : 127.0.0.1 (My computer internal IP address) // 165.246.243.47 현재 IP 주소입력하기
	Server_Address.sin_addr.s_addr = inet_addr("165.246.235.5");	// IP address
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
		
		double x1=0; // 상수입력을 받는 variable
		double x2=0; // 상수입력을 받는 variable
		char eq;

		// char Tmp_0[MAX_SIZE] = "Connect", Tmp_1[MAX_SIZE] = "Get", Tmp_2[MAX_SIZE] = "Put", Tmp_3[MAX_SIZE] = "Hello Server", Tmp_4[MAX_SIZE] = "Off", Tmp_5[MAX_SIZE] = "Good bye"; // 여기 부분 수정하기
		

		// string을 나눈다.입력 

		x1 = double(Buffer[0])-48;
		eq = Buffer[1];
		x2 = double(Buffer[2])-48;

		// 입이
		printf("%lf \n", x1); 
		printf("%lf \n", x2);
		printf("%c \n", eq);
		
		
		int SendSize = MAX_SIZE;
		int ActualSendSize;
		

		
		printf("%s", typeid(x1).name());
		printf("%lf", calculator(x1,x2,eq));
		
		// Sends a response to Client message.
		// If message is Unknown Command, Send a error message.
		// 채팅 메세지 추가할 것

		
		if ( typeid(x1)==typeid(double) && typeid(x2)==typeid(double) ){ // 계산식이 맞다면, 결과값을 돌려준다.
			char SendMessage[MAX_SIZE];
			sprintf_s(SendMessage, "%e", calculator(x1, x2, eq));
			
			ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
		}
		
		
		else { //계산식이 아니라면 에러를 돌려준다
			char SendMessage[MAX_SIZE] = "올바른 계산식을 입력해주세요.";
			ActualSendSize = send(Client_Socket, SendMessage, SendSize, 0);
		}
		
	}
	
	// Wait : Server state is listen.
	getchar();
}