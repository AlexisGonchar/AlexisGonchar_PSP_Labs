using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ServerConsole
{
    class Program
    {
        private static int port = 4444;
        private static TcpListener server;
        private static IPAddress ip;
        static void Main(string[] args)
        {
            Console.Write("Введите IP адрес: ");
            ip = IPAddress.Parse(Console.ReadLine());

            server = new TcpListener(ip, port);
            server.Start();
            Console.WriteLine("Сервер запущен. Ожидание подключений ...");

            while (true)
            {
                // получаем входящее подключение
                TcpClient client = server.AcceptTcpClient();
                Console.WriteLine("Подключен клиент. Выполнение запроса...");
                ClientObject clientobject = new ClientObject(client);

                Thread clientThread = new Thread(new ThreadStart(clientobject.Process));
                clientThread.Start();
            }

            Console.ReadLine();
        }
    }
}
