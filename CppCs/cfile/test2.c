#include "test2.h"

int add(int a, int b){
    return a + b;
}

// void *check_velocity(void *data){
//     int cnt = 0;
//     while(1){
//         printf("check_velocity()...\n");
//         sleep(1);
//         cnt++;
//         if(cnt > 10){
//             break;
//         }
//     }
// }

// void myThread1(){
//     pthread_t p_thread12;
//     int status12;
//     pthread_create(&p_thread12, NULL, check_velocity, NULL);
//     pthread_join(p_thread12, (void **)&status12);
//     printf("END...\n");
// }

void justPrint(){
    printf("justPrint()...\n");
}
