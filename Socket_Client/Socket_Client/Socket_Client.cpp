#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#include <windows.h>
#include <stdio.h>

int main()
{
    WSADATA wsaData;
    int retWSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (!retWSAStartup) {
        SOCKET hSock = socket(PF_INET, SOCK_STREAM, 0);

        SOCKADDR_IN servAddr;
        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servAddr.sin_port = htons(10000);

        if (!connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr))) {
            while (true) {
                char recvmsg[1024];
                if (recv(hSock, recvmsg, sizeof(recvmsg) - 1, 0) != SOCKET_ERROR) {
                    printf("recvMsg = %s\n", recvmsg);
                }
                else {
                    printf("recv Failed! Error = %d\n", WSAGetLastError());
                    closesocket(hSock);
                    WSACleanup();
                    return 1;
                }
            }
        }
        else {
            printf("connect Failed! Error = %d\n", WSAGetLastError());
            closesocket(hSock);
            WSACleanup();
            return 1;
        }

        closesocket(hSock);
        WSACleanup();
    }
    else {
        printf("WSAStartup Failed! Error = %d\n", retWSAStartup);
    }

    return 0;
}