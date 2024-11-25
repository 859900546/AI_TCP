#include "TcpClient.h"
#include <iostream>

TcpClient::TcpClient(const char* ip, int port)
{
	strcpy_s(this->ip, ip);
	this->port = port;
}

TcpClient::~TcpClient()
{
	closesocket(this->sock);
}

bool TcpClient::Connect()
{
	WSADATA wsaData;
	struct sockaddr_in serverAddr;

	// Initialize Winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cout << "WSAStartup failed with error: " << WSAGetLastError() << std::endl;
		return false;
	}

	// Create socket
	if (this->ConnectType == 1) {
		this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else {
		this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	if (this->sock == INVALID_SOCKET) {
		std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	// Set server address
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	inet_pton(AF_INET, this->ip, &serverAddr.sin_addr);

	if (this->ConnectType == 1) {
		// Set socket options
		int optval = 1;
		setsockopt(this->sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	}
	else {
		// Connect to server
		if (connect(this->sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
			closesocket(this->sock);
			std::cout << "Connection failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			return false;
		}
	}
	return true;
}


bool TcpClient::Send(std::vector<BYTE> data)
{
	if (this->sock == INVALID_SOCKET) {
		return false;
	}
	int size = data.size();
	int sent = send(this->sock, reinterpret_cast<const char*>(data.data()), size, 0);
	if (sent == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool TcpClient::Recv(char* data, int len)
{
	int recvd = recv(this->sock, data, len, 0);
	if (recvd == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

bool TcpClient::Sendall(std::vector<BYTE> data)
{
	if (this->sock == INVALID_SOCKET) {
		return false;
	}
	int total_sent = 0;
	int size = data.size();
	int bytes_left = size;
	int bytes_sent;
	//char *length = reinterpret_cast<char*>(&size);
	//send(this->sock, length, 4, 0);
	char* buffer = reinterpret_cast<char*>(data.data());
	int cnt = 0;
	while (total_sent < size) {
		bytes_sent = send(this->sock, buffer + total_sent, bytes_left, 0);
		if (bytes_sent == SOCKET_ERROR) {
			return false;
		}
		total_sent += bytes_sent;
		bytes_left -= bytes_sent;
	}
	return true;
}

bool TcpClient::getpoint(char* message, int * ans)
{
	ans[5] = 0;
	if (strcmp(message, "QC") == 0)
		return false;

	int res = 0;
	int flag = 1;
	int p = 0;
	for (int i = 0; message[i]; i++)
	{
		if (message[i] == 'Q' || message[i] == 'C')
			continue;
		if (message[i] == ',')
			ans[p++] = res * flag, res = 0,flag = 1;
		else if (message[i] >= '0' && message[i] <= '9' || message[i] == '-')
			if (message[i] == '-')
				flag = -1;
			else
				res = res * 10 + (message[i] - '0');
	}
	if (p == 4)
		ans[p] = res;
	else
		return false;
	ans[5] = 1;
	return true;
}
