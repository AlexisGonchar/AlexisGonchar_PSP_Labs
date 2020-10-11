#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <vector>
#include <ctime>
#include <sys/select.h>
#include <iostream>
using namespace std;

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

int nclients = 0;

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

int main() {
  // create a socket
  int server_socket = 0;
  int checkSelect, checkBind, checkListen, checkSend, checkSockopt;
  fd_set master; // create a master set
  fd_set clients; // create set for clients
  printf("TCP SERVER \n");
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

  // define the server address
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;

  FD_ZERO(&master); // zero the set

  // (Domain, Type of socket, Protocol)
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket == -1){
    perror("socket");
    exit(1);
  }

  // bind the socket to our specified IP and port
  server_address.sin_family = AF_INET; // // knows what type of server_address
  server_address.sin_port = htons(9002); // htons() converts data format so that our struct can understand the port number
  server_address.sin_addr.s_addr = INADDR_ANY; // bind to any available address
  memset(server_address.sin_zero, '\0', sizeof server_address.sin_zero); // for padding, to make it the same size as struct sockaddr

  int yes = 1;
  checkSockopt = setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if(checkSockopt == -1){
    perror("setsockopt");
    exit(1);
  }

  checkBind = bind(server_socket, (struct sockaddr *) &server_address, sizeof(struct sockaddr));
  if(checkBind == -1){
    perror("unable to bind");
    exit(1);
  }

  // begin listening for clients attempting to connect
  checkListen = listen(server_socket, CountOfClients);
  if(checkListen == -1){
    perror("listen");
    exit(1);
  }

  fflush(stdout);

  //printf("server socket: \t%d\n", server_socket);
  FD_SET(server_socket, &master); // sets the bit for the server_socket in the master socket set
  printf("Ожидание подключений...\n");
  //My
  socklen_t addrlen;
  int client_socket;
  addrlen = sizeof(struct sockaddr_in);
  vector<int> client_sockets(CountOfClients);
  while(client_socket = accept(server_socket, (struct sockaddr *)&client_address, &addrlen)){
	  client_sockets[nclients] = client_socket;
	  nclients++; // увеличиваем счетчик подключившихся клиентов
	  // пытаемся получить имя хоста
	  hostent* hst;
	  hst = gethostbyaddr((char*)&client_address.sin_addr.s_addr, 4, AF_INET);

	  // вывод сведений о клиенте
	  printf("+%s [%s] new connect!\n",
		  (hst) ? hst->h_name : "", inet_ntoa(client_address.sin_addr));
	  if (nclients == CountOfClients) {
		  start_time = clock();
		  for (int i = 0; i < CountOfClients; i++) {
			  send(client_sockets[i], (char*)&PartSize, sizeof(int), 0);
			  send(client_sockets[i], (char*)&arr[i][0], sizeof(int) * PartSize, 0);
			  int bytes_recv;
			  bytes_recv = recv(client_sockets[i], (char*)&arr[i][0], sizeof(int) * PartSize, 0);
			  nclients--; // уменьшаем счетчик активных клиентов
			  printf("-disconnect\n");
			  {
				  // закрываем сокет
				  close(client_sockets[i]);
			  }
		  }
		  final();
	  }
  }
  //EndMy

close(server_socket);
return 0;
}
