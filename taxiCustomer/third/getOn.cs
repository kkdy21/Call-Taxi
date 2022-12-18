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


namespace taxiCustomer.third
{
    public partial class getOn : Form
    {
        private NetworkStream stream;
        private thirdPage third;
        public getOn(thirdPage third, NetworkStream stream)
        {
            InitializeComponent();
            this.third = third;
            this.stream = stream;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[1024];
            buf = Encoding.ASCII.GetBytes("review_C");
            stream.Write(buf, 0, buf.Length);

            delay(100);

            buf = new byte[1024];
            string data = textBox1.Text;
            buf = Encoding.ASCII.GetBytes(data);
            stream.Write(buf, 0, buf.Length);


            byte[] buf3 = Encoding.UTF8.GetBytes("EndThread");
            stream.Write(buf3, 0, buf.Length);

            this.Close();
            third.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[1024];
            buf = Encoding.ASCII.GetBytes("getOff_C");
            stream.Write(buf, 0, buf.Length);


            this.panel4.BringToFront();
            this.panel4.Visible = true;
        }

        private void getOn_Load(object sender, EventArgs e)
        {
            int buf_len;
            byte[] buf = new byte[1024];
            string data = "getOn_C";
            delay(100);
            buf = Encoding.UTF8.GetBytes(data);
            stream.Write(buf, 0, buf.Length);

            buf = new byte[1024];
            buf_len = stream.Read(buf, 0, buf.Length);
            data = Encoding.UTF8.GetString(buf, 0, buf_len);
            Console.WriteLine(data);
            string[] words = data.Split('@');

            StringBuilder builder = new StringBuilder();
            
            builder.Append(words[0]);
            builder.AppendLine("에서");
            builder.Append(words[1]);
            builder.AppendLine("가는 택시를 탔습니다.");
            builder.AppendLine();
            builder.Append("대연택시, 차량번호 : ");
            builder.AppendLine(words[2]);
            builder.Append("예상 소요 시간은 ");
            builder.Append(words[3]);
            builder.AppendLine(" 분 입니다.");
            this.label4.Text = builder.ToString();
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
