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
    public partial class signUp : Form
    {
        private NetworkStream stream;
        public signUp(NetworkStream stream)
        {
            InitializeComponent();
            this.stream = stream;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void btnSignup_Click(object sender, EventArgs e)
        {


            if (txtName.Text == "" || txtID.Text == "" || txtPw.Text == "" || txtCheck.Text == "")
            {
                lblwarning.Text = "빈 칸을 채워주세요";
            }
            else if (txtPw.Text != txtCheck.Text)
            {
                lblwarning.Text = "비밀번호가 일치하지 않습니다.";
                txtCheck.Clear();
                txtPw.Clear();
            }
            else
            {
                string data = "signUp_C";
                byte[] buf = new byte[1024];

                buf = new byte[1024];
                buf = Encoding.ASCII.GetBytes(data);

                stream.Write(buf, 0, buf.Length);           // 플래그 
                delay(100);


                buf = new byte[1024];
                data = txtName.Text + "@" + txtID.Text + "@" + txtPw.Text;
                buf = Encoding.ASCII.GetBytes(data);
                stream.Write(buf, 0, buf.Length);           // 이름@아이디@비밀번호

                buf = new byte[1024];
                int len = stream.Read(buf, 0, buf.Length);            // 중복여부 또는 가입완료
                data = Encoding.UTF8.GetString(buf, 0,len);

                Console.WriteLine(data);
              
                if (data == "done")     //가입완료
                {
                    this.Close();
                }
                else
                {
                    lblwarning.Text = "중복된 아이디입니다.";
                    txtID.Clear();
                }

            }
        }

        private void txtName_Enter(object sender, EventArgs e)
        {
            if (txtName.Text == " 이름")
            {
                txtName.Text = "";
                txtName.ForeColor = Color.Black;
            }
        }

        private void txtName_Leave(object sender, EventArgs e)
        {
            if (txtName.Text == "")
            {
                txtName.Text = " 이름";
                txtName.ForeColor = Color.DarkGray;
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

        private void txtPw_Enter(object sender, EventArgs e)
        {
            if (txtPw.Text == " 비밀번호")
            {
                txtPw.Text = "";
                txtPw.ForeColor = Color.Black;
                txtPw.PasswordChar = '●';
            }
        }

        private void txtPw_Leave(object sender, EventArgs e)
        {
            if (txtPw.Text == "")
            {
                txtPw.Text = " 비밀번호";
                txtPw.ForeColor = Color.DarkGray;
                txtPw.PasswordChar = default;
            }
        }

        private void txtCheck_Enter(object sender, EventArgs e)
        {
            if (txtCheck.Text == " 비밀번호 확인")
            {
                txtCheck.Text = "";
                txtCheck.ForeColor = Color.Black;
                txtCheck.PasswordChar = '●';
            }
        }

        private void txtCheck_Leave(object sender, EventArgs e)
        {
            if (txtCheck.Text == "")
            {
                txtCheck.Text = " 비밀번호 확인";
                txtCheck.ForeColor = Color.DarkGray;
                txtCheck.PasswordChar = default;
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
