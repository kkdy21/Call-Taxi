using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net.Sockets;

namespace taxiCustomer
{
    public partial class firstPage : Form
    {
        private TcpClient client;
        private NetworkStream stream;
        public firstPage()
        {
            InitializeComponent();
            client = new TcpClient("10.10.20.49", 5001);
            stream = client.GetStream();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            first.signIn signIn = new first.signIn(this, stream);
            signIn.TopLevel = false;
            panel1.Controls.Add(signIn);
            signIn.BringToFront();
            signIn.Show();
        }

        private void btnsignUp_Click(object sender, EventArgs e)
        {
            first.signUp signup = new first.signUp(stream);
            signup.TopLevel = false;
            panel1.Controls.Add(signup);
            signup.BringToFront();
            signup.Show();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
