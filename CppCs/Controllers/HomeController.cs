using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using CppCs.Models;
using System.Runtime.InteropServices;
using System.Threading;


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

        [DllImport("./cfile/test.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void init();

        [DllImport("./cfile/test.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void print();

        [DllImport("./cfile/test.dll", CallingConvention = CallingConvention.Cdecl)]
        static extern void main();

        #region threadtest
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void run_thread1();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void run_thread2();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void run_thread3();

        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void stop_thread1();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void stop_thread2();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void stop_thread3();

        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void isrun_thread1();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void isrun_thread2();
        [DllImport("./cfile/threadtest/conveyor1", CallingConvention = CallingConvention.Cdecl)]
        static extern void isrun_thread3();
        #endregion

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
            //startscrew(0, null);

            string str;
            str = "Hello World...! " + add(1, 2) + ' ' + add(3, 4);
            //str = "Hello World...! " + 3;

            //ioread();

            //init();
            //Console.WriteLine("init() done...");
            //Thread.Sleep(1);
            //print();
            //Console.WriteLine("print() done...");

            //main();



            #region threadtest
            
            isrun_thread1();
            Thread.Sleep(1);
            run_thread1();
            Thread.Sleep(5000);
            isrun_thread1();
            Thread.Sleep(1);

            Console.WriteLine("stop thread..");
            stop_thread1();
            Thread.Sleep(1);
            isrun_thread1();
            
            #endregion



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
