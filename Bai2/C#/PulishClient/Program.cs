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
            client.MqttMsgPublished += Client_MqttMsgPublished;
            ushort msgId = client.Publish("/my_test", Encoding.UTF8.GetBytes(@"{
                ""name"": ""Pham Hong Phuc"",
                ""age"": 20,
                ""address"": ""Hai Duong""
            }"), 
             MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
        }

        private static void Client_MqttMsgPublished(object sender, MqttMsgPublishedEventArgs e)
        {
            MqttClient client = sender as MqttClient;
            Console.WriteLine("MessageId = " + e.MessageId + " Published = " + e.IsPublished);
        }
    }
}
