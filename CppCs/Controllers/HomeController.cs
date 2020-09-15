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
        [DllImport("./cfile/libtest4", CallingConvention = CallingConvention.Cdecl)]
        static extern int add(int a, int b);

        //[DllImport("~/cfile/test2.dll", CallingConvention = CallingConvention.Cdecl)]
        //static extern void myThread1();

        [DllImport("./cfile/libtest4", CallingConvention = CallingConvention.Cdecl)]
        static extern void justPrint();

        [DllImport("./cfile/screwtest/libscrew", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        static extern void startscrew(int argc, string[] argv);

        [DllImport("./cfile/libioread", CallingConvention = CallingConvention.Cdecl)]
        static extern void ioread();

        private readonly ILogger<HomeController> _logger;

        public HomeController(ILogger<HomeController> logger)
        {
            _logger = logger;
        }

        public string Index()
        {
            //return View();
            
            justPrint();

            //myThread1();
            startscrew(0, null);

            string str;
            str = "Hello World...! " + add(1, 2) + ' ' + add(3, 4);
            //str = "Hello World...! " + 3;

            ioread();

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
