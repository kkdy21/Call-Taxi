using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using CefSharp;
using CefSharp.WinForms;
using System.Drawing.Imaging;
using System.IO;
using System.Net.Sockets;


namespace taxiCustomer
{

    public partial class mainPage : Form
    {
        private Bitmap image;
        private NetworkStream stream;

        public mainPage(NetworkStream stream)
        {
            InitializeComponent();
            InitializeChromeBrowser();
            this.stream = stream;
        }

        private void InitializeChromeBrowser()
        {
            //CefSettings cefSettings = new CefSettings();
            //Cef.Initialize(cefSettings);
            ChromiumWebBrowser chrome = new ChromiumWebBrowser(@"C:\Users\hpark\Desktop\taxiCustomer\taxiApi.html");
            chrome.Dock = DockStyle.Fill;
            pictureBox1.Controls.Add(chrome);


            chrome.JavascriptObjectRepository.Settings.LegacyBindingEnabled = true;
            chrome.JavascriptObjectRepository.Register("cAPI", new msgFromHtml(), false, BindingOptions.DefaultBinder);

        }

        private void btnRequest_Click(object sender, EventArgs e)
        {
            string data = null;
            byte[] buf = new byte[1024];

            data = msgFromHtml.msg;
            if (data != null)
            {
                Bitmap bitmap = CaptureControl(this, pictureBox1);
                using (MemoryStream stream = new MemoryStream())
                {
                    bitmap.Save(stream, ImageFormat.Jpeg);
                    byte[] imageByte = stream.ToArray();        // 이미지를 byte화. 이것을 서버로전송하면됨.



                  
                }
                this.image = bitmap;                            // 캡쳐한 이미지 저장

                buf = new byte[1024];
                buf = Encoding.ASCII.GetBytes("call_C");      // call 
                stream.Write(buf, 0, buf.Length);

                delay(1000);
                buf = new byte[1024];
                buf = Encoding.UTF8.GetBytes(data);
                stream.Write(buf, 0, buf.Length);           // 출발지@목적지@거리@소요시간@요금
                string test = Encoding.UTF8.GetString(buf);
                Console.WriteLine(test);
                this.Hide();
                thirdPage t = new thirdPage(image, stream, test);
                t.ShowDialog();
                this.Show();
            }
            else
            {
                MessageBox.Show("출발지, 도착지를 설정 후 확인을 눌러주세요.", "TAXI");
            }
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            this.Close();
        }




        public Bitmap CaptureControl(Form form, Control control)        // 캡쳐
        {
            Bitmap bitmap = new Bitmap(control.Width - 275, control.Height - 100);
            Graphics graphics = Graphics.FromImage(bitmap);
            Size size = new Size(pictureBox1.Width - 275, pictureBox1.Height - 100);
            graphics.CopyFromScreen(form.PointToScreen(new Point(control.Left + 207, control.Top + 110)), new Point(0, 0), size);
            graphics.Save();
            return bitmap;
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            history h = new history(stream);
            h.TopLevel = false;
            panelAll.Controls.Add(h);
            h.BringToFront();
            h.Show();
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

    internal class msgFromHtml
    {
        public static string msg;

        public msgFromHtml()
        {
            msg = null;
        }

        public void showMsg(string data)
        {
            msg = data;
        }
    }
}
