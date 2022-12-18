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
using System.Threading;

namespace taxiCustomer.third
{
    public partial class matched : Form
    {
        private NetworkStream stream;
        private Bitmap bitmap;
        private Thread t;
        private string Driver;
        public matched(Bitmap image, NetworkStream stream, string Driver)
        {
            InitializeComponent();
            this.bitmap = image;
            this.stream = stream;
            this.Driver = Driver;
        }

        private void matched_Load(object sender, EventArgs e)
        {
            int buf_len;
            Label[] lab = { lblKM, lblFare, lblTime, lblDriver, lblVNum };
            int cnt = 0;
            string data = "take_C";
            byte[] buf = new byte[1024];

            delay(80);

            buf = new byte[1024];
            buf = Encoding.UTF8.GetBytes(data);
            stream.Write(buf, 0, buf.Length);           // 플래그

            delay(80);

            buf = new byte[1024];
            buf = Encoding.UTF8.GetBytes(Driver);
            stream.Write(buf, 0, buf.Length);           // 기사 아이디

            buf = new byte[1024];
            buf_len = stream.Read(buf, 0, buf.Length);            //  총거리@택시요금@소요시간@기사님@택시번호
            data = Encoding.UTF8.GetString(buf, 0, buf_len);
          
            string[] words = data.Split('@');
            foreach (string word in words)
            {
                lab[cnt++].Text = word;
            }

            pictureBox1.Image = bitmap;
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
