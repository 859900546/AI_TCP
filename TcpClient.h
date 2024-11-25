#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class TcpClient {
public:
    TcpClient(const char* ip, int port);
    ~TcpClient();
    bool Connect();
    bool Send(std::vector<BYTE> data);
    bool Sendall(std::vector<BYTE> data);
    bool Recv(char* data, int len);
    bool getpoint(char* message,int* ans);
private:
    SOCKET sock = INVALID_SOCKET;
    char ip[15];
    int port;
    int ConnectType = 0;   // 0:TCP, 1:UDP
};