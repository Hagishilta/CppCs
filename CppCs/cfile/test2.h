#ifndef TEST2_H
#define TEST2_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifdef BUILD_MY_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif

int add(int a, int b);
void *check_velocity(void *data);
void myThread1();
void justPrint();

#endif
