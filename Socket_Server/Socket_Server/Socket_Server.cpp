#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <WS2tcpip.h>

#include <windows.h>
#include <stdio.h>

using namespace std;
#include <string>

void clientHandler(SOCKET* hClntSock) {
    while (true) {
        Sleep(1000);
        char sendmsg[] = "SendTest";
        int retSend = send(*hClntSock, sendmsg, sizeof(sendmsg), 0);
        if (retSend == SOCKET_ERROR) {
            printf("send Failed! Error = %d\n", WSAGetLastError());
            closesocket(*hClntSock);
            return;
        }
    }
}

int main()
{
    WSADATA wsaData;
    int retWSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (!retWSAStartup) {
        SOCKET hServSock = socket(PF_INET, SOCK_STREAM, 0);

        SOCKADDR_IN servAddr;
        memset(&servAddr, 0, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servAddr.sin_port = htons(10000);

        if (!bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr))) {
            if (!listen(hServSock, 10)) {
                SOCKADDR_IN clntAddr;
                int size_clntAddr = sizeof(clntAddr);

                while (true) {
                    SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &size_clntAddr);
                    if (hClntSock != INVALID_SOCKET) {
                        string clntIp;
                        inet_ntop(AF_INET, &(clntAddr.sin_addr), (PSTR)&clntIp, INET_ADDRSTRLEN);

                        printf("New Client Connected! IP = %s\n", clntIp.c_str());

                        DWORD threadId;
                        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)clientHandler, &hClntSock, 0, &threadId);
                    }
                    else {
                        printf("accept Failed! Error = %d\n", WSAGetLastError());
                        closesocket(hClntSock);
                        closesocket(hServSock);
                        WSACleanup();
                        return 1;
                    }
                }
            }
            else {
                printf("listen Failed! Error = %d\n", WSAGetLastError());
                closesocket(hServSock);
                WSACleanup();
                return 1;
            }
        }
        else {
            printf("bind Failed! Error = %d\n", WSAGetLastError());
            closesocket(hServSock);
            WSACleanup();
            return 1;
        }

        closesocket(hServSock);
        WSACleanup();
    }
    else {
        printf("WSAStartup Failed! Error = %d\n", retWSAStartup);
    }

    return 0;
}