﻿// Пример простого TCP-эхо-сервера

#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>
#include <locale.h>
#include <iostream>
#include <regex>
using namespace std;


LPTHREAD_START_ROUTINE WINAPI NewClient(PVOID* argt);
#define MY_PORT 666 // Порт, который слушает сервер 666
const unsigned int MAX_BUF_LENGTH = 4096;

char buff[1024];

int main()
{
    cout << "SERVER APPLICATION \n";

    // Шаг 1 - Инициализация Библиотеки Сокетов
    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        // Ошибка!
        printf("Error WSAStartup %d\n", WSAGetLastError());
        return -1;
    }

    // Шаг 2 - создание сокета
    SOCKET mysocket;
    // AF_INET - сокет Интернета
    // SOCK_STREAM - потоковый сокет (с установкой соединения)
    // 0 - по умолчанию выбирается TCP протокол
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        // Ошибка!
        printf("Error socket %d\n", WSAGetLastError());
        WSACleanup(); // Деиницилизация библиотеки Winsock
        return -1;
    }

    // Шаг 3 - связывание сокета с локальным адресом
    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT); // не забываем о сетевом порядке!!!
    local_addr.sin_addr.s_addr = 0; // сервер принимает подключения
                                    // на все свои IP-адреса

    // вызываем bind для связывания
    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {
        // Ошибка
        printf("Error bind %d\n", WSAGetLastError());
        closesocket(mysocket); // закрываем сокет!
        WSACleanup();
        return -1;
    }

    // Шаг 4 - ожидание подключений
    // размер очереди - 0x100
    if (listen(mysocket, 0x100))
    {
        // Ошибка
        printf("Error listen %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }

    cout << "SERVER IS READY!\n";
    cout << "Waiting for connections...\n";

    // Шаг 5 - извлекаем сообщение из очереди
    SOCKET client_socket; // сокет для клиента
    sockaddr_in client_addr; // адрес клиента (заполняется системой)

    // функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);

    // цикл извлечения запросов на подключение из очереди
    while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, \
        & client_addr_size)))
    {
        
        // пытаемся получить имя хоста
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        // вывод сведений о клиенте
        printf("+%s [%s] new connect!\n",
            (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
        
        QueueUserWorkItem((LPTHREAD_START_ROUTINE)NewClient, &client_socket, WT_EXECUTEDEFAULT);
        
        
    }
    return 0;
}

LPTHREAD_START_ROUTINE WINAPI NewClient(PVOID* argt)
{
    SOCKET client_socket = ((SOCKET*)argt)[0];
    char message[MAX_BUF_LENGTH];

    regex regextext("^(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[-_]).{6,}$");
    regex regexkir("[а-яА-Я]");
    regex regexzn("[/*+=!@#$%^,.<>;:()&|\`~{}]");

    while (strcmp(message, "quit") != 0) {
        bool correct = false;
        int bytes_recv;
        bytes_recv = recv(client_socket, &message[0], MAX_BUF_LENGTH, 0);
        cout << message << endl;
        if (regex_match(message, regextext)) {
            if (!regex_match(message, regexkir) && !regex_match(message, regexzn)) {
                correct = true;
            }
        }

        if (correct) {
            strcpy_s(message, "Valid");
        }
        else {
            strcpy_s(message, "Invalid");
        }
        send(client_socket, &message[0], MAX_BUF_LENGTH, 0);
    }
    printf("-disconnect\n");
    // закрываем сокет
    closesocket(client_socket);
    return 0;
}
