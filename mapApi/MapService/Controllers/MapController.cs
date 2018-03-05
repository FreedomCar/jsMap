using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using MapService.Models;
using Microsoft.AspNetCore.Mvc;

namespace MapService.Controllers
{
    [Route("api/MapController")]
    public class MapController : Controller
    {
        [HttpPost]
        public string Post([FromBody] Points points)
        {
            points.fromLng = 118.951388;
            points.fromLat = 32.123387;
            points.toLng = 118.951388;
            points.toLat = 32.923387;
            var client = new HttpClient();
            var uri = "http://restapi.amap.com/v4/direction/bicycling?origin=" + points.fromLng + "," + points.fromLat +
                      "&destination=" + points.toLng + "," + points.toLat +
                      "&output=json&key=9e312862ef2950b0474dbd7a62797047";
            var resMsg = client.GetAsync(uri);
            var msg = resMsg.Result.Content.ReadAsStringAsync();
            return msg.Result;
        }
        
        [HttpGet]
        public string Get()
        {
            var points = new Points
            {
                fromLng = 118.951388,
                fromLat = 32.123387,
                toLng = 118.951388,
                toLat = 32.923387
            };
            var client = new HttpClient();
            var uri = "http://restapi.amap.com/v4/direction/bicycling?origin=" + points.fromLng + "," + points.fromLat +
                      "&destination=" + points.toLng + "," + points.toLat +
                      "&output=json&key=9e312862ef2950b0474dbd7a62797047";
            var resMsg = client.GetAsync(uri);
            var msg = resMsg.Result.Content.ReadAsStringAsync();
            return msg.Result;
        }
    }
}