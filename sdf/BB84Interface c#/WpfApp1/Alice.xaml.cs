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
    public partial class Alice : Window
    {

        int refreshRate = 1000;
        bool start = false;

        public Alice()
        {

            InitializeComponent();

            
            new Thread(doSome).Start();

            //

            //one thread per changing value (different refresh rate)

            //doSome();


        }
        
        public void doSome()
        {
            while (!start) { }

            for (int i = 0; i < 1000; i++)
            {
                textBlock.Dispatcher.BeginInvoke(new Action(() =>
                {
                    textBlock.Text = i.ToString();
                    
                    //((Alice)System.Windows.Application.Current.MainWindow).UpdateLayout();
                }));
                Thread.Sleep(refreshRate);
            }
            
            
        }

        

        private void BttnTCP_Click(object sender, RoutedEventArgs e)
        {
            TCPconfig winTCP = new TCPconfig(this);
            winTCP.Show();
            this.Hide();
        }

        private void BttnProtocol_Click(object sender, RoutedEventArgs e)
        {
            ProtocolConfig winProtocol = new ProtocolConfig(this);
            winProtocol.Show();
            this.Hide();
        }

        private void BttnStart_Click(object sender, RoutedEventArgs e)
        {
            //get ip address and tcp port
            start = true;
        }

        private void BttnRefresh_Click(object sender, RoutedEventArgs e)
        {
            refreshRate = Int32.Parse(textBox.Text);
        }
    }
}
