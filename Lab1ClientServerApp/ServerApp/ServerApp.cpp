// Пример простого TCP-эхо-сервера

#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // Wincosk2.h должен быть раньше windows!
#include <windows.h>
#include <locale.h>
#include <iostream>
#include <vector>

#define MY_PORT 666 // Порт, который слушает сервер 666

// макрос для печати количества активных пользователей
#define PRINTNUSERS if (nclients) printf("%d user on-line\n", nclients); \
        else if(arrayscount == -1 && clientscount == 4) final();



std::vector<int> merge(std::vector<int> a1, std::vector<int> a2, int size) {
    const int a1l = size;
    const int a2l = size;
    const int a3l = size * 2;
    std::vector<int> a3(size * 2);
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

// прототип функции, обслуживающий подключившихся пользователей
DWORD WINAPI SexToClient(LPVOID client_socket);

// глобальная переменная - количество активных пользователей
int nclients = 0;
int arr[4][250];
int arrres[1000];
int clientscount;
int arrayscount;


void final() {
    std::vector<std::vector<int> > m;
    m.reserve(18);
    for (std::size_t i = 0; i != 4; ++i)
        m.push_back(std::vector<int>(arr[i], arr[i] + 250));
    std::vector<int> result = merge(merge(m[0], m[1], 250), merge(m[2], m[3], 250), 500);
    for (std::vector<int>::iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << *it;
        std::cout << "\n";
    }
        
}

int main()
{
    char buff[1024];
    setlocale(0, "");
    int i = 0;
    int j = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 250; j++) {
            arr[i][j] = rand() % 1000;
        }
    }
    clientscount = 0;
    arrayscount = 3;

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

    // цикл извлечения запросов на подключение из очереди
    while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, \
        & client_addr_size)))
    {
        nclients++; // увеличиваем счетчик подключившихся клиентов

        // пытаемся получить имя хоста
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

        // вывод сведений о клиенте
        printf("+%s [%s] new connect!\n",
            (hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
        PRINTNUSERS

            // Вызов нового потока для обслужвания клиента
            // Да, для этого рекомендуется использовать _beginthreadex
            // но, поскольку никаких вызовов функций стандартной Си библиотеки
            // поток не делает, можно обойтись и CreateThread
            DWORD thID;
        CreateThread(NULL, NULL, SexToClient, &client_socket, NULL, &thID);
    }
    return 0;
}

// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
DWORD WINAPI SexToClient(LPVOID client_socket)
{
    SOCKET my_sock;
    my_sock = ((SOCKET*)client_socket)[0];

    // отправляем клиенту приветствие
    send(my_sock, (char *)arr[clientscount], sizeof(int)*250, 0);
    clientscount++;

    // цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
    int bytes_recv;
    while ((bytes_recv = recv(my_sock, (char *)&arr[arrayscount][0], sizeof(int)*250, 0)) &&
        bytes_recv != SOCKET_ERROR)
    {
        arrayscount--;
    }

    // если мы здесь, то произошел выход из цикла по причине
    // возращения функцией recv ошибки - соединение с клиентом разорвано
    nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n"); PRINTNUSERS
    {
        // закрываем сокет
        closesocket(my_sock);
    }

        
    return 0;
}