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
using System.Drawing.Imaging;
using System.IO;


namespace taxiCustomer
{
    public partial class thirdPage : Form
    {
        private int duration;
        private Bitmap image;
        private NetworkStream stream;
        public Thread t;

        public thirdPage(Bitmap bitmap, NetworkStream stream, string place)
        {
            InitializeComponent();
            duration = 0;
            timer1.Interval = 1000;
            this.image = bitmap;
            this.stream = stream;

            string[] lab = place.Split('@');
            label9.Text = lab[0];
            label10.Text = lab[1];

            timer1.Start();
            t = new Thread(() => response(stream));
            t.Start();
        }


        public void response(NetworkStream stream)
        {
            byte[] buf = new byte[1024];
            string data = null;
            int buf_len;


            buf = new byte[1024];
            Console.WriteLine("accept 전");
            buf_len = stream.Read(buf, 0, buf.Length);
            data = Encoding.UTF8.GetString(buf, 0, buf_len);

            if (data == "accept")
            {
                Console.WriteLine("accept 들어옴");

                delay(100);

                timer1.Stop();
                buf = new byte[1024];

                buf_len = stream.Read(buf, 0, buf.Length);
                string temp = Encoding.UTF8.GetString(buf, 0, buf_len);
                //MessageBox.Show(temp);

                // 이미지전송 
                buf = new byte[1024];
                buf = Encoding.ASCII.GetBytes("map_C");
                stream.Write(buf, 0, buf.Length);

                delay(100);
                buf = new byte[1024];
                buf = Encoding.UTF8.GetBytes(temp);
                stream.Write(buf, 0, buf.Length);


                using (MemoryStream Mstream = new MemoryStream())
                {
                    Bitmap map = this.image;
                    map.Save(Mstream, ImageFormat.Jpeg);

                    byte[] imagebyte = Mstream.ToArray();


                    byte[] len = Encoding.UTF8.GetBytes(imagebyte.Length.ToString());
                    stream.Write(len, 0, len.Length);
                    delay(100);
                    stream.Write(imagebyte, 0, imagebyte.Length);
                }


                third.matched m = new third.matched(image, stream, temp);

                panelAll.Invoke(new MethodInvoker(delegate ()
                   {
                       m.TopLevel = false;
                       panelAll.Controls.Add(m);
                       m.BringToFront();
                       m.Show();
                   }));



                buf = new byte[1024];
                Console.WriteLine("read대기");
                buf_len = stream.Read(buf, 0, buf.Length);                    // 쓰레드 만들어야 할 수도 있음
                data = Encoding.ASCII.GetString(buf, 0, buf_len);
                Console.WriteLine(data);
                //if (data == "geton")
                //{
                    Console.WriteLine("geton받음");
                    m.Invoke(new MethodInvoker(delegate ()
                   {
                       m.Close();
                   }));

                    third.getOn g = new third.getOn(this, stream);
                    panelAll.Invoke(new MethodInvoker(delegate ()
                    {
                        g.TopLevel = false;
                        panelAll.Controls.Add(g);
                        g.BringToFront();
                        g.Show();
                    }));
                //}
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            byte[] buf = new byte[1024];

            buf = Encoding.ASCII.GetBytes("cancel_C");
            stream.Write(buf, 0, buf.Length);

            this.Close();
        }



        private void timer1_Tick(object sender, EventArgs e)
        {
            duration++;
            lblSec.Text = duration.ToString();

            if (duration == 20)
            {
                byte[] buf = new byte[1024];

                buf = Encoding.ASCII.GetBytes("failed_C");
                stream.Write(buf, 0, buf.Length);

                timer1.Stop();
                lblFailed.BringToFront();
                lblFailed.Visible = true;
                delay(3000);
                this.Close();
            }
        }

        private void thirdPage_FormClosing(object sender, FormClosingEventArgs e)
        {
            byte[] buf = Encoding.UTF8.GetBytes("EndThread");
            stream.Write(buf, 0, buf.Length);
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
