#include "conveyor1.h"

#ifndef _MAX_PATH
    #define _MAX_PATH 260
#endif

#ifndef DEBUG
    #define DEBUG 1
#endif

const int SIZE_IO = 32;
const int SIZE_IN = 22;
const int SIZE_OUT = 26;
char* FILE_INPUT = "/cfile/conveyor1PinIn.txt";
char* FILE_OUTPUT = "/cfile/conveyor1PinOut.txt";
const int NUM_HARDSTOPPER = 21;
const int NUM_BOWLSENSOR = 21;

bool IS_RUNNING = FALSE;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t p_thread_stop;
int status_stop;

typedef struct input{
    char name[50];
    int number;
} Input;

typedef struct output{
    char name[50];
    int number;
} Output;

Input inputs[22];
Output outputs[26];

bool is_running_conveyor1(){
    pthread_mutex_lock(&mutex);
    if(IS_RUNNING){
        return true;
    }
    else{
        return false;
    }
    pthread_mutex_unlock(&mutex);
}

void setup_raspberry_pi(){
    if(DEBUG){
        printf("raspberry pi SETUP START...\n");
    }
    wiringPiSetup();

    mcp23017Setup(100, 0x20);   // IO expander

    for(int i = 0; i < SIZE_IO; ++i){
        pinMode(100 + i, INPUT);
        pullUpDnControl(100 + i, PUD_UP);   // or PUD_DOWN
    }
    for(int i = SIZE_IO; i < 2*SIZE_IO; ++i){
        pinMode(100 + i, OUTPUT);
    }

    // GPIO output : Power LED & Buzzer
    for(int i=0; i<4; ++i){
        pinMode(outputs[i].number, OUTPUT);
    }

    // power LED ON
    digitalWrite(outputs[0].number, 0);     // R
    digitalWrite(outputs[1].number, 1);     // G
    digitalWrite(outputs[2].number, 0);     // B

    if(DEBUG){
        printf("raspberry pi SETUP DONE...\n");
    }
}

void init_conveyor1(){
    if(DEBUG){
        printf("Conveyor1 SETUP START...\n");
    }
    pthread_mutex_lock(&mutex);
    IS_RUNNING = true;

    char strBuffer[_MAX_PATH];
	char* pstrBuffer = NULL;
	pstrBuffer = getcwd( strBuffer, _MAX_PATH );
    FILE_INPUT = strcat(strBuffer, FILE_INPUT);

    char strBuffer2[_MAX_PATH];
	char* pstrBuffer2 = NULL;
	pstrBuffer2 = getcwd( strBuffer2, _MAX_PATH );
    FILE_OUTPUT = strcat(strBuffer2, FILE_OUTPUT);

    FILE * fp;
    fp = fopen(FILE_INPUT, "rt");
    for(int i=0; i<SIZE_IN; ++i){
        fscanf(fp, "%s : %d", inputs[i].name, &inputs[i].number);
        if(DEBUG){
            printf("%d th %s value => %d \n", i, inputs[i].name, inputs[i].number);
        }
    }
    fclose(fp);

    fp = fopen(FILE_OUTPUT, "rt");
    for(int i=0; i<SIZE_OUT; ++i){
        fscanf(fp, "%s : %d", outputs[i].name, &outputs[i].number);
        if(DEBUG){
            printf("%d th %s value => %d \n", i, outputs[i].name, outputs[i].number);
        }
    }
    fclose(fp);

    setup_raspberry_pi();

    // conveyor STOP
    digitalWrite(outputs[4].number, 0);

    // hardstopper OPEN
    for(int i=0; i<NUM_HARDSTOPPER; ++i){
        digitalWrite(outputs[i + 5], 1);
    }

    IS_RUNNING = false;
    pthread_mutex_unlock(&mutex);
    if(DEBUG){
        printf("Conveyor1 SETUP END...\n");
    }
}

void *conveyor1_mode_detector(void *data){
    int prevMode = 0;
    int currMode = 0;
    while(TRUE){
        currMode = digitalRead(inputs[21].number);
        if(currMode != prevMode){
            if(currMode == 0){  // STOP MODE
                digitalWrite(outputs[4].number, 0);
                for(int i=0; i<NUM_HARDSTOPPER; ++i){
                    digitalWrite(outputs[i + 5], 1);
                }
            }
            else if(currMode > 0){  // AUTO MODE
                // run by KMS
            }
            else if(currMode < 0){  // MANUAL MODE
                for(int i=0; i<NUM_HARDSTOPPER; ++i){
                    digitalWrite(outputs[i + 5], 1);
                }
                digitalWrite(outputs[4].number, 1);
            }
            else{
                // do nothing
            }
        }
        prevMode = currMode;
        sleep(1);
    }
}

void run_conveyor1(int num_hardstopper){
    if(num_hardstopper < 0 || num_hardstopper > 21){
        return;
    }

    if(DEBUG){
        printf("Conveyor1 START...\n");
    }
    
    pthread_mutex_lock(&mutex);
    IS_RUNNING = true;    

    digitalWrite(outputs[num_hardstopper + 4].number, 0);
    
    digitalWrite(outputs[4].number, 1);

    while(TRUE){
        if(conveyor2_isbowl_sensor(num_distributor)){
            // sleep(1);
            break;
        }
        sleep(1);
    }
    
    for(int i=0; i<NUM_DISTRIBUTOR; ++i){
        digitalWrite(outputs[i + 5], 1);
    }

    IS_RUNNING = false;
    pthread_mutex_unlock(&mutex);

    if(DEBUG){
        printf("Conveyor1 DONE...\n");
    }
}

bool conveyor1_isbowl_sensor_all(){
    for(int i=1; i<=NUM_BOWLSENSOR; ++i){
        if(conveyor1_isbowl_sensor(i)){
            return TRUE;
        }
    }
    return FALSE;
}

bool conveyor1_isbowl_sensor(int num_position){
    if(0 < num_position && num_position < 22){
        int isBowl = digitalRead(inputs[num_position - 1].number);
        if(isBowl){
            return TRUE;
        }
        else{
            return FALSE;
        }
    }
    else{
        return FALSE;
    }
}

int conveyor1_get_current_mode(){
    int mode = digitalRead(inputs[21].number);

    return mode;
    
    // if(mode == 0){
    //     return 0;
    // }
    // else if(mode < 0){
    //     return -1;
    // }
    // else if(mode > 0){
    //     return 1;
    // }
    // else{
    //     return 0;
    // }
}


int test_value1 = 0;
int test_value2 = 0;
int test_value3 = 0;
pthread_t p_thread_1;
int status_1;
pthread_t p_thread_2;
int status_2;
pthread_t p_thread_3;
int status_3;

void run_thread1(){
    pthread_create(&p_thread_1, NULL, test_thread1, NULL);
    // pthread_join(p_thread_1, (void **)&status_1);
}
void run_thread2(){
    pthread_create(&p_thread_2, NULL, test_thread2, NULL);
}
void run_thread3(){
    pthread_create(&p_thread_3, NULL, test_thread3, NULL);
}

void stop_thread1(){
    pthread_cancel(p_thread_1);
}

void stop_thread2(){
    pthread_cancel(p_thread_2);
}

void stop_thread3(){
    pthread_cancel(p_thread_3);
}

void isrun_thread1(){
    pthread_mutex_lock(&mutex);
    if(test_value1){
        printf("test thread1 running...\n");
    }
    else{
        printf("test thread1 NOT running...\n");
    }
    pthread_mutex_unlock(&mutex);
}
void isrun_thread2(){
    pthread_mutex_lock(&mutex);
    if(test_value2){
        printf("test thread2 running...\n");
    }
    else{
        printf("test thread2 NOT running...\n");
    }
    pthread_mutex_unlock(&mutex);
}
void isrun_thread3(){
    pthread_mutex_lock(&mutex);
    if(test_value3){
        printf("test thread3 running...\n");
    }
    else{
        printf("test thread3 NOT running...\n");
    }
    pthread_mutex_unlock(&mutex);
}

void *test_thread1(void *data){
    pthread_mutex_lock(&mutex);
    test_value1 = 1;
    pthread_mutex_unlock(&mutex);
    while(1){
        printf("thread 1 running...\n");
    }
}

void *test_thread2(void *data){
    pthread_mutex_lock(&mutex);
    test_value2 = 1;
    pthread_mutex_unlock(&mutex);
    while(1){
        printf("thread 2 running...\n");
    }
}

void *test_thread3(void *data){
    pthread_mutex_lock(&mutex);
    test_value3 = 1;
    pthread_mutex_unlock(&mutex);
    while(1){
        printf("thread 3 running...\n");
    }
}


int legacy(int argc, char *argv[])
{
    if(DEBUG){
        printf("----- bowl program start -----\n");
    }
    
    // pthread_t p_thread00;
    // int status00;
    // pthread_create(&p_thread00, NULL, demo, NULL);
    // pthread_join(p_thread00, (void **)&status00);

    while (1) {
        switch (inputNum)
        {
        case 81:
            if(DEBUG){
                printf("execution 0 (Dispose ready)...\n");
            }

            digitalWrite(PIN_NOZZLE, 1);
            sleep(2);
            
            TMC5130_rotate(PISTON_PIN, PISTON_HOME_VELOCITY);
    
            pthread_t p_thread0;
            int status0;
            pthread_create(&p_thread0, NULL, homing, NULL);
            pthread_join(p_thread0, (void **)&status0);

            TMC5130_rotate(PISTON_PIN, 0);

            sleep(1);

            digitalWrite(PIN_VALVE, 1);
            sleep(1);
            
            digitalWrite(PIN_NOZZLE, 0);
            
            break;    

        case 78:
            if(DEBUG){
                printf("execution 1 (Loading / Disposing)...\n");
            }

            TMC5130_rotate(PISTON_PIN, 0);
            tmc5130_writeInt(PISTON_PIN, TMC5130_VMAX, PISTON_VELOCITY);
            TMC5130_moveTo(PISTON_PIN, PISTON_MAXI);

            pthread_t p_thread12;
            int status12;
            pthread_create(&p_thread12, NULL, check_velocity, NULL);
            pthread_join(p_thread12, (void **)&status12);

            valve();

            break;
        
        case 79:
            if(DEBUG){
                printf("execution 2 (Stirring ON)...\n");
            }
            // valve(); // just for a testing
            digitalWrite(PIN_STIRRING, 1);

            break;

        case 80:
            if(DEBUG){
                printf("execution 3 (Stirring OFF)...\n");
            }

            digitalWrite(PIN_STIRRING, 0);

            break;    
        
        case 74:
            if(DEBUG){
                printf("execution 4 (Dispensing 1)...\n");
            }
            TMC5130_rotate(PISTON_PIN, 0);
            tmc5130_writeInt(PISTON_PIN, TMC5130_VMAX, PISTON_VELOCITY);
            TMC5130_moveTo(PISTON_PIN, PISTON_AMOUNT_1);
            pthread_t p_thread42;
            int status42;
            pthread_create(&p_thread42, NULL, check_velocity, NULL);
            pthread_join(p_thread42, (void **)&status42);

            valve();

            break;

        case 75:
            if(DEBUG){
                printf("execution 5 (Dispensing 2)...\n");
            }

            TMC5130_rotate(PISTON_PIN, 0);
            tmc5130_writeInt(PISTON_PIN, TMC5130_VMAX, PISTON_VELOCITY);
            TMC5130_moveTo(PISTON_PIN, PISTON_AMOUNT_2);
            pthread_t p_thread52;
            int status52;
            pthread_create(&p_thread52, NULL, check_velocity, NULL);
            pthread_join(p_thread52, (void **)&status52);

            valve();
            
            break;

        case 76:
            if(DEBUG){
                printf("execution 6 (Dispensing 3)...\n");
            }

            TMC5130_rotate(PISTON_PIN, 0);
            tmc5130_writeInt(PISTON_PIN, TMC5130_VMAX, PISTON_VELOCITY);
            TMC5130_moveTo(PISTON_PIN, PISTON_AMOUNT_3);
            pthread_t p_thread62;
            int status62;
            pthread_create(&p_thread62, NULL, check_velocity, NULL);
            pthread_join(p_thread62, (void **)&status62);

            valve();
            
            break;

        case 70:
            if(DEBUG){
                printf("execution 7 (motor testing)...\n");
                sleep(3);
            }

            break;
        
        case 71:
            if(DEBUG){
                printf("execution 8...\n");    
            }
            
            break;
        
        case 72:
            if(DEBUG){
                printf("execution 9 (Clean ready)...\n");    
            }
            digitalWrite(PIN_NOZZLE, 1);
            sleep(1);

            digitalWrite(PIN_VALVE, 0);
            sleep(1);

            TMC5130_rotate(PISTON_PIN, 0);
            tmc5130_writeInt(PISTON_PIN, TMC5130_VMAX, PISTON_VELOCITY);
            TMC5130_moveTo(PISTON_PIN, PISTON_EXIT);
            pthread_t p_thread91;
            int status91;
            pthread_create(&p_thread91, NULL, check_velocity, NULL);
            pthread_join(p_thread91, (void **)&status91);
            
            break;

        default:
            break;
        }
    }

    return 0;
}
