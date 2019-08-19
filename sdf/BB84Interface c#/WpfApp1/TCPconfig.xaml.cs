using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class TCPconfig : Window
    {
        private Alice alice;
        string remoteIpAddress = "127.0.0.1";
        int tcpPort = 8080;

        public TCPconfig(Alice alice)
        {
            this.alice = alice;
            InitializeComponent();

            //new Thread(doSome).Start();

            //doSome();


        }

        private string getRemoteIpAddres()
        {
            return remoteIpAddress;
        }

        private int getTcpPort()
        {
            return tcpPort;
        }
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            alice.Show();
            this.Close();
        }

        private void Button1_Click(object sender, RoutedEventArgs e)
        {

        }

        private void bttnSubmitTCP_Click(object sender, RoutedEventArgs e)
        {
            remoteIpAddress = textBoxIpAddress.Text;
            tcpPort = Int32.Parse(textBoxTCP.Text);
            MessageBox.Show("Configuration updated.");
        }
        


    }
}
