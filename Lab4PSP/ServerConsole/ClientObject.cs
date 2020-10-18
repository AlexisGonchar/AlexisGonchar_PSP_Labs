﻿using System;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace ServerConsole
{
    public class ClientObject
    {
        private TcpClient client;
        private NetworkStream stream = null;
        private string clientName = string.Empty;

        public ClientObject(TcpClient client)
        {
            this.client = client;
        }

        public void Process()
        {
            try
            {
                stream = client.GetStream();
                byte[] data = new byte[64];
                StringBuilder builder = new StringBuilder();

                while (builder.ToString() != "quit")
                {
                    builder.Clear();
                    int bytes = 0;
                    do
                    {
                        bytes = stream.Read(data, 0, data.Length);
                        builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
                    }
                    while (stream.DataAvailable);

                    var read_string = builder.ToString().Split(';');
                    Console.WriteLine("Имя: " + read_string[0]);
                    Console.WriteLine("Пароль: " + read_string[1]);

                    
                    if (!ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc), read_string[1]))
                    {
                        Console.WriteLine(clientName + "поток не создан");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                if (stream != null)
                    stream.Close();
                if (client != null)
                    client.Close();
                Console.ReadKey();
            }
        }

        void ThreadProc(Object pass)
        {
            string password = (string)pass;
            var answer = Encoding.Unicode.GetBytes(password + ": NORM");
            stream.Write(answer, 0, answer.Length);
        }
    }
}
