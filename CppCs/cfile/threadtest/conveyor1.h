#ifndef CONVEYOR1_H
#define CONVEYOR1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/select.h>
#include <termios.h>
#include <sysexits.h>
#include <glob.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <wiringPi.h>
#include <mcp23017.h>



#ifdef BUILD_MY_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif



typedef enum {false, true} bool;

bool is_running_conveyor1();
void setup_raspberry_pi();
void init_conveyor1();
void *conveyor1_mode_detector(void *data);
void run_conveyor1(int num_hardstopper);
void *stop_conveyor1(void *data);
bool conveyor1_isbowl_sensor_all();
bool conveyor1_isbowl_sensor(int num_position);
int conveyor1_get_current_mode();

void run_thread1();
void run_thread2();
void run_thread3();
void stop_thread1();
void stop_thread2();
void stop_thread3();
void isrun_thread1();
void isrun_thread2();
void isrun_thread3();
void *test_thread1(void *data);
void *test_thread2(void *data);
void *test_thread3(void *data);



int legacy(int argc, char *argv[]);

#endif
