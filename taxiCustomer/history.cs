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
    public partial class history : Form
    {
        private NetworkStream stream;
        public history(NetworkStream stream)
        {
            InitializeComponent();
            this.stream = stream;

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void history_Load(object sender, EventArgs e)
        {
            Panel[] pnl = { panel1, panel2, panel3 };
            Label[] lab = { lbl1, lbl2, lbl3, lbl4, lbl5, lbl6, lbl7, lbl8, lbl9, lbl10, lbl11, lbl12 };
            Label[] lab2 = { lblCall, lblCount, lblTotal };
            int labCnt = 0;
            int lab2cnt = 0;
            int len;
            string data;

            data = "history_C";
            byte[] buf = new byte[1024];
            buf = Encoding.UTF8.GetBytes(data);
            stream.Write(buf, 0, buf.Length);

            buf = new byte[2048];
            int buflen = stream.Read(buf, 0, buf.Length);
            string tempCount = Encoding.UTF8.GetString(buf, 0, buflen);
            int count = Int32.Parse(tempCount);
            Console.WriteLine(count);

           

            for (int i = 0; i < count; i++)
            {
                buf = new byte[1024];
                len = stream.Read(buf, 0, buf.Length);
                data = Encoding.UTF8.GetString(buf, 0, len);
                Console.WriteLine(data);
                string[] words = data.Split('@');
                foreach (string word in words)
                {
                    lab[labCnt++].Text = word;
                }
                pnl[i].Visible = true;      
            }
            if (count == 0)
                label6.Visible = true;

            buf = new byte[1024];
            len = stream.Read(buf, 0, buf.Length);
            data = Encoding.UTF8.GetString(buf, 0, len);
            Console.WriteLine(data);
            string[] vals = data.Split('@');
            foreach (string val in vals)
            {
                lab2[lab2cnt++].Text = val;
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
