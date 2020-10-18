using System;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace NetworkPoolLib
{
    public class Client
    {
        public string UserName { get; set; }
        TcpClient client = null;
        int port = 4444;
        string ip = string.Empty;
        NetworkStream stream;
        public Client(string name, string ip)
        {
            UserName = name;
            this.ip = ip;

            client = new TcpClient(ip, port);
            stream = client.GetStream();
        }

        public string Send(string password)
        {
            byte[] data = Encoding.Unicode.GetBytes(UserName + ";" + password);
            stream.Write(data, 0, data.Length);

            ThreadPool.QueueUserWorkItem(new WaitCallback(ThreadProc));
            data = new byte[64];
            StringBuilder builder = new StringBuilder();
            int bytes = 0;
            do
            {
                bytes = stream.Read(data, 0, data.Length);
                builder.Append(Encoding.Unicode.GetString(data, 0, bytes));
            }
            while (stream.DataAvailable);

            string answ = builder.ToString();
            return answ;
        }

        private void ThreadProc(object state)
        {
            
        }
    }
}
