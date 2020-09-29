// Пример простого TCP-эхо-сервера

#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>
#include <locale.h>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

#define MY_PORT 666 // Порт, который слушает сервер 666



vector<int> merge(vector<int> a1, vector<int> a2) {
    const int a1l = a1.size();
    const int a2l = a2.size();
    const int a3l = a1l + a2l;
    vector<int> a3(a3l);
    int i = 0, j = 0;
    for (int k = 0; k < a3l; k++) {

        if (i > a1l - 1) {
            int a = a2[j];
            a3[k] = a;
            j++;
        }
        else if (j > a2l - 1) {
            int a = a1[i];
            a3[k] = a;
            i++;
        }
        else if (a1[i] < a2[j]) {
            int a = a1[i];
            a3[k] = a;
            i++;
        }
        else {
            int b = a2[j];
            a3[k] = b;
            j++;
        }
    }
    return a3;
}

// глобальная переменная - количество активных пользователей
int nclients = 0;


int arrres[1000];
vector<vector<int>> arr;
int CountOfClients;
int ArraySize;
int PartSize;
long start_time, end_time;


void final() {
    vector<int> result(0);
    for (int i = 0; i < CountOfClients; i++) {
        result = merge(result, arr[i]);
    }
    end_time = clock() - start_time;
    cout << end_time;
    cout << " ms\n";
    for (vector<int>::iterator it = result.begin(); it != result.end(); ++it) {
        cout << *it;
        cout << "\n";
    }
        
}


int main()
{
    cout << "Enter the number of clients:\n";
    cin >> CountOfClients;
    do {
        cout << "Enter the number of items:\n";
        cin >> ArraySize;
    } while ((ArraySize % CountOfClients) != 0);
    
    PartSize = ArraySize / CountOfClients;
    char buff[1024];
    setlocale(0, "");
    arr = vector<vector<int>>(CountOfClients, vector<int>(PartSize));
    for (int i = 0; i < CountOfClients; ++i) {
        for (int j = 0; j < PartSize; ++j) {
            arr[i][j] = rand() % 1000 + 1;
        }
    }

    printf("TCP SERVER \n");
    // Шаг 1 - Инициализация Библиотеки Сокетов
    // т.к. возвращенная функцией информация не используется
    // ей передается указатель на рабочий буфер, преобразуемый к указателю
    // на структуру WSADATA.
    // Такой прием позволяет сэкономить одну переменную, однако, буфер
    // должен быть не менее полкилобайта размером (структура WSADATA
    // занимает 400 байт)
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

    printf("Ожидание подключений...\n");

    // Шаг 5 - извлекаем сообщение из очереди
    SOCKET client_socket; // сокет для клиента
    sockaddr_in client_addr; // адрес клиента (заполняется системой)

    // функции accept необходимо передать размер структуры
    int client_addr_size = sizeof(client_addr);

    vector<SOCKET> client_sockets(CountOfClients);
    // цикл извлечения запросов на подключение из очереди
    while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, \
        & client_addr_size)))
    {
        client_sockets[nclients] = client_socket;
        nclients++; // увеличиваем счетчик подключившихся клиентов

        // пытаемся получить имя хоста
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        // вывод сведений о клиенте
        printf("+%s [%s] new connect!\n",
            (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));


            // Вызов нового потока для обслужвания клиента
            // Да, для этого рекомендуется использовать _beginthreadex
            // но, поскольку никаких вызовов функций стандартной Си библиотеки
            // поток не делает, можно обойтись и CreateThread
            DWORD thID;
            start_time = clock();
        if (nclients == CountOfClients) {
            for (int i = 0; i < CountOfClients; i++) {
                send(client_sockets[i], (char*)&PartSize, sizeof(int), 0);
                send(client_sockets[i], (char*)&arr[i][0], sizeof(int) * PartSize, 0);
                int bytes_recv;
                bytes_recv = recv(client_sockets[i], (char*)&arr[i][0], sizeof(int) * PartSize, 0);
                nclients--; // уменьшаем счетчик активных клиентов
                printf("-disconnect\n"); 
                {
                    // закрываем сокет
                    closesocket(client_sockets[i]);
                }
            }
            final();
        }
        
        
    }
    return 0;
}
