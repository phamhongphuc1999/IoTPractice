using System;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;

namespace Bai1
{
    class Program
    {
        public static async Task<string> GetWebContent(string url, string requestContent = null)
        {
            HttpClient httpClient = new HttpClient();
            try
            {
                HttpRequestMessage requestMessage = new HttpRequestMessage(HttpMethod.Post, url);
                if(requestContent != null)
                    requestMessage.Content = new StringContent(requestContent, Encoding.UTF8, "application/json");
                HttpResponseMessage response = await httpClient.SendAsync(requestMessage);
                string rcontent = await response.Content.ReadAsStringAsync();
                return rcontent;
            }
            catch(Exception e)
            {
                return e.Message;
            }
        }

        static void Main(string[] args)
        {
            string url = "http://203.171.20.94:8012/api/AccessControl/GetUserInfor";
            string content = @"
                {
                    ""deviceId"":""8a0fc66a61a959f6"",
                    ""qrCodeId"":""a652d57094a7590b9dea115b156c07098abde87"",
                    ""qrCodeValue"":""P22498244182551944""
                }";
            Task<string> result = Program.GetWebContent(url, content);
            result.Wait();
            Console.WriteLine(result.Result);
        }
    }
}
