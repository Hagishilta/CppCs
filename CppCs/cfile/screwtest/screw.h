#include "SPI.h"
#include "TMC5160.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		// usleep
#include <stdint.h>
#include <wiringPi.h>	// wiringPi
#include <pthread.h>	// threading

#ifdef BUILD_MY_DLL
    #define SHARED_LIB __declspec(dllexport)
#else
    #define SHARED_LIB __declspec(dllimport)
#endif

void *thread1(void *data);
void *thread2(void *data);
int main9(void);
void startscrew(int argc, char **argv);
