using NetworkPoolLib;
using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Documents;

namespace ClientUI
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        Client client;
        List<string> consoleText;
        public MainWindow()
        {
            InitializeComponent();
            consoleText = new List<string>();
        }

        private void buttonConnect_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string userName = textBoxUserName.Text;
                string ip = textBoxIpAddress.Text;
                client = new Client(userName, ip);
            }catch(Exception exp)
            {
                WriteToConsole( exp.Message);
            }
        }

        private void WriteToConsole(string text)
        {
            if (consoleText.Count > 8)
                consoleText.RemoveAt(0);
            consoleText.Add(text);
            StringBuilder builder = new StringBuilder();
            foreach(string str in consoleText)
            {
                builder.Append(DateTime.Now.ToString() + ": " + str + "\n");
            }
            textBlock.Text = builder.ToString();
        }

        private void button_Copy_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string pass = textBoxPassword.Text;
                string answ = client.Send(pass);
                WriteToConsole(answ);
            }
            catch (Exception exp)
            {
                WriteToConsole(exp.Message);
            }
        }
    }
}
