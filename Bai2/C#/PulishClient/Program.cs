using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace PulishClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Pulished Client\n");
            MqttClient client = new MqttClient("broker.hivemq.com");
            byte code = client.Connect(Guid.NewGuid().ToString());

            //add event
            client.MqttMsgPublished += Client_MqttMsgPublished;
            client.ConnectionClosed += Client_ConnectionClosed;

            while (true)
            {
                string message = Console.ReadLine();
                if (message == "")
                {
                    client.Publish("/my_test", Encoding.UTF8.GetBytes(""), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
                    client.Disconnect();
                    break;
                }
                ushort msgId = client.Publish("/my_test", Encoding.UTF8.GetBytes(message),
             MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
            }
        }

        private static void Client_ConnectionClosed(object sender, EventArgs e)
        {
            Console.WriteLine("Client is disconnected");
        }

        private static void Client_MqttMsgPublished(object sender, MqttMsgPublishedEventArgs e)
        {
            Console.WriteLine("MessageId = " + e.MessageId + " Published = " + e.IsPublished);
        }
    }
}
