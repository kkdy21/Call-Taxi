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

namespace taxiCustomer.first
{
    public partial class signIn : Form
    {
        private firstPage first;
        private NetworkStream stream;
        public signIn(firstPage first, NetworkStream stream)
        {
            InitializeComponent();
            this.first = first;
            this.stream = stream;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void txtPW_Enter(object sender, EventArgs e)
        {
            if (txtPW.Text == " 비밀번호")
            {
                txtPW.Text = "";
                txtPW.ForeColor = Color.Black;
                txtPW.PasswordChar = '●';
            }
        }

        private void txtPW_Leave(object sender, EventArgs e)
        { 
            if (txtPW.Text == "")
            {
                txtPW.Text = " 비밀번호";
                txtPW.ForeColor = Color.DarkGray;
                txtPW.PasswordChar = default;
            }
        }

        private void txtID_Enter(object sender, EventArgs e)
        {
            if (txtID.Text == " 아이디")
            {
                txtID.Text = "";
                txtID.ForeColor = Color.Black;
            }
        }

        private void txtID_Leave(object sender, EventArgs e)
        {
            if (txtID.Text == "")
            {
                txtID.Text = " 아이디";
                txtID.ForeColor = Color.DarkGray;
            }
        }

        private void btnSignIn_Click(object sender, EventArgs e)
        {
            if (txtID.Text == "" || txtPW.Text == "")
            {
                lblWarning.Text = "빈 칸을 채워주세요";
            }
            else
            {
                int len;
                string data = "signIn_C";
                byte[] buf = new byte[1024];
                buf = Encoding.ASCII.GetBytes(data);            // 플래그
                stream.Write(buf, 0, buf.Length);

                delay(100);

                buf = new byte[1024];
                data = txtID.Text + "@" + txtPW.Text;
                buf = Encoding.ASCII.GetBytes(data);            // 아이디@비밀번호
                stream.Write(buf, 0, buf.Length);
                buf = new byte[1024];
                len = stream.Read(buf, 0, buf.Length);
                data = Encoding.Default.GetString(buf, 0, len);

                if (data == "done")
                {
                    first.Hide();
                    mainPage m = new mainPage(stream);
                    m.ShowDialog();
                    txtPW.PasswordChar = default;
                    txtPW.Text = " 비밀번호";
                    txtPW.ForeColor = Color.DarkGray;
                    txtID.Text = " 아이디";
                    txtID.ForeColor = Color.DarkGray;
                    lblWarning.Text = "";
                    first.Show();
                }
                else
                {
                    lblWarning.Text = "아이디와 비밀번호를 확인해주세요";
                }

            }
        }

        private static DateTime delay(int MS)
        {
            DateTime ThisMoment = DateTime.Now;
            TimeSpan duration = new TimeSpan(0, 0, 0, 0, MS);
            DateTime AfterWards = ThisMoment.Add(duration);

            while (AfterWards >= ThisMoment)
            {
                System.Windows.Forms.Application.DoEvents();
                ThisMoment = DateTime.Now;
            }
            return DateTime.Now;
        }
    }
}
