// udp_discover.cpp
// Simple UDP discovery client for Windows (Winsock)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define DISCOVERY_MSG "DISCOVER"
#define PORT 50000
#define BROADCAST_IP "255.255.255.255"
#define BUF_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // Enable broadcast
    BOOL broadcastEnable = TRUE;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "setsockopt (SO_BROADCAST) failed." << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    sockaddr_in broadcast_addr;
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(PORT);
    broadcast_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    // Send discovery message
    if (sendto(sockfd, DISCOVERY_MSG, strlen(DISCOVERY_MSG), 0, (sockaddr*)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
        std::cerr << "sendto failed." << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    std::cout << "Discovery message sent. Waiting for responses..." << std::endl;

    // Set socket to non-blocking mode
    u_long mode = 1;
    ioctlsocket(sockfd, FIONBIO, &mode);

    sockaddr_in recv_addr;
    int addr_len = sizeof(recv_addr);
    char buffer[BUF_SIZE];

    DWORD start = GetTickCount();
    while (GetTickCount() - start < 1000) { // 1 second window
        int n = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0, (sockaddr*)&recv_addr, &addr_len);
        if (n == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAEWOULDBLOCK) {
                Sleep(10); // Sleep briefly to avoid busy-waiting
                continue;
            } else {
                break; // Other error
            }
        }
        buffer[n] = '\0';
        std::cout << "Received from " << inet_ntoa(recv_addr.sin_addr) << ": " << buffer << std::endl;
    }

    closesocket(sockfd);
    WSACleanup();
    std::cout << "Done." << std::endl;
    return 0;
}
