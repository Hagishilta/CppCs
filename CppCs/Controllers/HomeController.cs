using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using CppCs.Models;
using System.Runtime.InteropServices;


namespace CppCs.Controllers
{
    public class HomeController : Controller
    {
        //"C:\\Users\\John Lee\\Desktop\\AvokadoLab\\2020\\Test1\\CppCs\\CppCs\\cfile\\test2.dll"
        [DllImport("./cfile/libtest3.so", CallingConvention = CallingConvention.Cdecl)]
        static extern int add(int a, int b);

        //[DllImport("~/cfile/test2.dll", CallingConvention = CallingConvention.Cdecl)]
        //static extern void myThread1();

        [DllImport("./cfile/libtest3.so", CallingConvention = CallingConvention.Cdecl)]
        static extern void justPrint();

        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public string Index()
        {
            //return View();
            string str;
            justPrint();

            //myThread1();

            str = "Hello World...! " + add(1, 2) + ' ' + add(3, 4);
            //int a = add(1, 2);
            //str = "Hello World...! " + 3;

            return str;
        }

        public IActionResult Privacy()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
