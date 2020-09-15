// http://wiringpi.com/extensions/i2c-mcp23008-mcp23017/

#ifdef BUILD_MY_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif

/*
 * q2w.c:
 *      Using the Quick 2 wire board for its mcp23017
 *
 * Copyright (c) 2012-2013 Gordon Henderson. <projects@drogon.net>
 ***********************************************************************
 */
#include <stdio.h>
#include <wiringPi.h>
#include <mcp23017.h>

int ioread()
{
  int i, bit ;

  wiringPiSetup();

  mcp23017Setup (100, 0x20) ;
  mcp23017Setup (200, 0x21) ;

  printf ("Raspberry Pi - MCP23017 Test\n") ;

  for (i = 0 ; i < 16 ; ++i){
    pinMode (100 + i, INPUT) ;
    pullUpDnControl(100 + i, PUD_UP);   // or PUD_DOWN

    pinMode (200 + i, INPUT) ;
    pullUpDnControl(200 + i, PUD_UP);
  }
    

  // pinMode         (100 + 15, INPUT) ;
  // pullUpDnControl (100 + 15, PUD_UP) ;
  
  for(int i=0;i<16;++i){
    int val = digitalRead(100+i);
    printf("0x20 %d-th value : %d\n", i, val);
  }
  for(int i=0;i<16;++i){
    int val1 = digitalRead(200+i);
    printf("0x21 %d-th value : %d\n", i, val1);
  }

//   for (;;)
//   {
//     for (i = 0 ; i < 1024; ++i)
//     {
//       for (bit = 0 ; bit < 10 ; ++bit)
//         digitalWrite (100 + bit, i & (1 << bit)) ;
//       delay (0.3) ;
//       printf("i bit read: %d %d %d\n", i, bit, );
//     }
//     printf("cycle...");
//   }

  return 0 ;
}
