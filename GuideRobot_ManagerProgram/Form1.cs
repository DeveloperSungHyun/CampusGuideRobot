using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Reflection.Emit;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace GuideRobot_ManagerProgram
{
    
    public partial class Form1 : Form
    {
        private static int V = 27;

        int totalDistance_1, totalDistance_2;

        public static int[] DijkstraAlgorithm(int[,] graph, int src, int dest)
        {
            int[] dist = new int[V];
            bool[] sptSet = new bool[V];
            int[] parent = new int[V];

            for (int i = 0; i < V; i++)
            {
                dist[i] = int.MaxValue;
                sptSet[i] = false;
            }

            dist[src] = 0;
            parent[src] = -1;

            for (int count = 0; count < V - 1; count++)
            {
                int u = MinDistance(dist, sptSet);

                sptSet[u] = true;

                for (int v = 0; v < V; v++)
                {
                    if (!sptSet[v] && graph[u, v] != 0 && dist[u] != int.MaxValue && dist[u] + graph[u, v] < dist[v])
                    {
                        parent[v] = u;
                        dist[v] = dist[u] + graph[u, v];
                    }
                }
            }

            List<int> path = new List<int>();
            int crawl = dest;
            path.Add(crawl);
            while (parent[crawl] != -1)
            {
                path.Add(parent[crawl]);
                crawl = parent[crawl];
            }

            path.Reverse();

            // 결과를 배열로 변환
            int[] result = new int[path.Count + 1];
            for (int i = 0; i < path.Count; i++)
            {
                result[i] = path[i];
            }
            result[path.Count] = dist[dest];

            return result;
        }

        private static int MinDistance(int[] dist, bool[] sptSet)
        {
            int min = int.MaxValue, minIndex = -1;

            for (int v = 0; v < V; v++)
            {
                if (!sptSet[v] && dist[v] <= min)
                {
                    min = dist[v];
                    minIndex = v;
                }
            }

            return minIndex;
        }
        int[,] weight = new int[27, 27]
        {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 48, 0, 0, 0, 0},
            {0, 0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 55, 0, 0, 0},
            {0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37, 47, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 29},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 50, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 50, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 0, 0, 0, 20, 43, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 38, 72, 72, 0, 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 30, 34, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 30, 37, 0, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {48, 0, 0, 70, 0, 47, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 34, 55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 36, 50, 43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 0, 0}
        };
        double[,] map = new double[27, 2]
        {
            {36.337906, 127.446377 }, //1_w1
            {36.339163, 127.447170 }, //2_w2
            {36.339200, 127.446601 }, //3_w3
            {36.338747, 127.446184 }, //4_w4
            {36.337895, 127.445145 }, //5_w5
            {36.337852, 127.445527 }, //6_w6
            {36.336849, 127.444824 }, //7_w7
            {36.337292, 127.444778 }, //8_w8
            {36.336881, 127.444483 }, //9_w9
            {36.336535, 127.444290 }, //10_w10
            {36.335781, 127.443646 }, //11_w11
            {36.335781, 127.443646 }, //12_w12
            {36.335831, 127.444215 }, //13_w13
            {36.335596, 127.444861 }, //14_w14
            {36.335676, 127.445188 }, //15_w15
            {36.335781, 127.443646 }, //16_w16
            {36.335717, 127.445534 }, //17_w17
            {36.336466, 127.445942 }, //18_w18
            {36.336764, 127.446098 }, //19_w19
            {36.336764, 127.446098 }, //20_101
            {36.336764, 127.446098 }, //21_102
            {36.336764, 127.446098 }, //22_103
            {36.336764, 127.446098 }, //23_104
            {36.336764, 127.446098 }, //24_105
            {36.336764, 127.446098 }, //25_106
            {36.336764, 127.446098 }, //26_107
            {36.336764, 127.446098 }, //26_108
        };

        int Battery;
        SerialPort SerialPort1 = new SerialPort();
        static Thread thread1 = new Thread(Update);
        public Form1()
        {
            InitializeComponent();


        }

        private void Form1_Load(object sender, EventArgs e)
        {
            thread1.Start();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
        }

        private void comboBox1_Click(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            foreach (var item in SerialPort.GetPortNames())
            {
                comboBox1.Items.Add(item);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (comboBox1.Text == "") return;
            try
            {
                if (SerialPort1.IsOpen)
                {
                    SerialPort1.Close();
                }
                else
                {
                    SerialPort1.PortName = comboBox1.SelectedItem.ToString();
                    SerialPort1.BaudRate = 9600;
                    SerialPort1.DataBits = 8;
                    SerialPort1.StopBits = StopBits.One;
                    SerialPort1.Parity = Parity.None;
                    SerialPort1.Open();
                }
            }catch (Exception) { 
                MessageBox.Show("연결에러", "알림", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            button3.Text = SerialPort1.IsOpen ? "연결해제" : "연결하기";
           comboBox1.Enabled = !SerialPort1.IsOpen;

        }

        static void Update()
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            String log_text = null;
            int src_1 = 0;  // 예시 출발지 노드(항산 도서관)
            int dest_1 = Convert.ToInt32(textBox1.Text) - 1;  // 호출지 노드

            int src_2 = dest_1;
            int dest_2 = Convert.ToInt32(textBox2.Text) - 1;  // 목적지 노드

            var result_1 = DijkstraAlgorithm(weight, src_1, dest_1);
            int[] path_1 = new int[result_1.Length - 1];
            Array.Copy(result_1, path_1, result_1.Length - 1);
            totalDistance_1 = result_1[result_1.Length - 1];

            var result_2 = DijkstraAlgorithm(weight, src_2, dest_2);
            int[] path_2 = new int[result_2.Length - 1];
            Array.Copy(result_2, path_2, result_2.Length - 1);
            totalDistance_2 = result_2[result_2.Length - 1];

            label3.Text = map[dest_1, 0].ToString();// 호출지 위도
            label4.Text = map[dest_1, 1].ToString();// 호출지 경도



            label5.Text = map[dest_2, 0].ToString();// 목적지 위도
            label6.Text = map[dest_2, 1].ToString();// 목적지 경도

            label11.Text = totalDistance_1 + " m";//호출지 까지 거리
            label16.Text = totalDistance_2 + " m";//호출지 에서 목적지 까지 거리

            log_text += "호출지\n";
            for (int i = 0; i < path_1.Length; i++)
            {
                log_text += path_1[i] + " -> " + map[path_1[i], 0] + ", " + map[path_1[i], 1] + "\n";
            }
            log_text += "목적지\n";
            for (int i = 0; i < path_2.Length; i++)
            {
                log_text += path_2[i] + " -> " + map[path_2[i], 0] + ", " + map[path_2[i], 1] + "\n";
            }

            Log_text.Text = log_text;

            if (!SerialPort1.IsOpen) return;
            SerialPort1.Write("0\n");
            for (int i = 0; i < path_1.Length; i++)
            {
                SerialPort1.Write(map[path_1[i], 0] + ", " + map[path_1[i], 1] + "\n");
            }
            SerialPort1.Write("1\n");
            for (int i = 0; i < path_2.Length; i++)
            {
                SerialPort1.Write(map[path_2[i], 0] + ", " + map[path_2[i], 1] + "\n");
            }

        }

        private void label11_Click(object sender, EventArgs e)
        {

        }
    }
}
