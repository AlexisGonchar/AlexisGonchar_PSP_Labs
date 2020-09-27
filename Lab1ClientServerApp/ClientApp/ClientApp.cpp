// Пример простого TCP-клиента
#include <stdio.h>
#include <string.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#define PORT 666
char SERVERADDR[11];

int main(int argc, char* argv[])
{
    char buff[1024];
    printf("TCP CLIENT\n");
    int choose;
    std::cout << "Choose:\n1.Local IP\n2.Enter IP\n";
    std::cin >> choose;
    switch (choose) 
    {
        case 1:
            strcpy_s(SERVERADDR, "127.0.0.1");
            break;
        case 2: 
            std::cout << "Enter server's ip address:";
            std::cin >> SERVERADDR;
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

    // Шаг 4 - чтение и передача сообщений
    const int arraysize = 250;
    int nsize;
    int res[arraysize];
    while ((nsize = recv(my_sock, (char*)res, arraysize * sizeof(int), NULL)) != SOCKET_ERROR)
    {
        nsize = nsize / sizeof(int);
        // ставим завершающий ноль в конце строки
        buff[nsize] = 0;
        int tmp;
        for (int i = 1; i < arraysize; i++)
            for (int j = i; j > 0 && res[j - 1] > res[j]; j--) 
            {
                tmp = res[j - 1];
                res[j - 1] = res[j];
                res[j] = tmp;
            }
                
        for (const auto& e : res) {
            std::cout << e << std::endl;
        }

        // читаем пользовательский ввод с клавиатуры
        //printf("S<=C:"); fgets(&buff[0], sizeof(buff) - 1, stdin);

        // проверка на "quit"
        /*
         if (!strcmp(&buff[0], "quit\n"))
        {
            // Корректный выход
            printf("Exit...");
            closesocket(my_sock);
            WSACleanup();
            return 0;
        }

        // передаем строку клиента серверу
        send(my_sock, &buff[0], strlen(&buff[0]), 0);
        */
       
    }
    printf("Recv error %d\n", WSAGetLastError());
    closesocket(my_sock);
    WSACleanup();
    return -1;
}