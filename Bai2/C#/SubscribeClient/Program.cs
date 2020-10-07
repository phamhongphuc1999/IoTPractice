using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace SubscribeClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Subscribed client");
            MqttClient client = new MqttClient("broker.hivemq.com");
            byte code = client.Connect(Guid.NewGuid().ToString());

            //add event
            client.MqttMsgSubscribed += Client_MqttMsgSubscribed;
            client.MqttMsgPublishReceived += Client_MqttMsgPublishReceived;

            ushort msgId = client.Subscribe(new string[] { "/my_test" }, new byte[] { MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE });
        }

        private static void Client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            Console.WriteLine("Received = " + Encoding.UTF8.GetString(e.Message) + " on topic " + e.Topic);
        }

        private static void Client_MqttMsgSubscribed(object sender, MqttMsgSubscribedEventArgs e)
        {
            Console.WriteLine("Subscribed for id = " + e.MessageId);
        }
    }
}
