// Пример простого TCP-клиента
#include <stdio.h>
#include <string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

#define PORT 666
const unsigned int MAX_BUF_LENGTH = 4096;

char SERVERADDR[11];

int main(int argc, char* argv[])
{
    char buff[1024];
    printf("CLIENT APPLICATION\n");
    int choose;
    cout << "Choose:\n1.Local IP\n2.Enter IP\n";
    cin >> choose;
    switch (choose)
    {
    case 1:
        strcpy_s(SERVERADDR, "127.0.0.1");
        break;
    case 2:
        cout << "Enter server's ip address:";
        cin >> SERVERADDR;
        break;
    }


    // Шаг 1 - инициализация библиотеки Winsock
    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        printf("WSAStart error %d\n", WSAGetLastError());
        return -1;
    }

    // Шаг 2 - создание сокета
    SOCKET my_sock;
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        printf("Socket() error %d\n", WSAGetLastError());
        return -1;
    }

    // Шаг 3 - установка соединения
    // заполнение структуры sockaddr_in - указание адреса и порта сервера
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT* hst;

    // преобразование IP адреса из символьного в сетевой формат
    if (inet_addr(SERVERADDR) != INADDR_NONE)
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
    {
        // попытка получить IP адрес по доменному имени сервера
        if (hst = gethostbyname(SERVERADDR))
            // hst->h_addr_list содержит не массив адресов,
            // а массив указателей на адреса
            ((unsigned long*)&dest_addr.sin_addr)[0] =
            ((unsigned long**)hst->h_addr_list)[0][0];
        else
        {
            printf("Invalid address %s\n", SERVERADDR);
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
    }

    // адрес сервера получен - пытаемся установить соединение
    if (connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr)))
    {
        printf("Connect error %d\n", WSAGetLastError());
        return -1;
    }

    printf("Connection to %s successfully\n \
            Type quit for quit\n\n", SERVERADDR);

    char message[MAX_BUF_LENGTH];

    while (strcmp(message, "quit") != 0) {
        cout << "Enter the message.\n";
        cin >> message;

        // Шаг 4 - чтение и передача сообщений
        int nsize;

        // передаем строку клиента серверу
        send(my_sock, &message[0], MAX_BUF_LENGTH, 0);

        recv(my_sock, &message[0], MAX_BUF_LENGTH, 0);
        cout << message << endl;
    }
    
    closesocket(my_sock);
    WSACleanup();
    int i;
    cin >> i;
    return 0;
}