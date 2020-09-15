#include "screw.h"

void *thread1(void *data){
    int id = *((int *) data);
    int i = 0;
    while(1){
        printf("thread # %d value : %d\n", id, i);
        ++i;
        usleep(500000);
    }
}

void *thread2(void *data){
    int id = *((int *) data);
    int i = 0;
    while(1){
        printf("thread # %d value : %d\n", id, i);
        ++i;
        usleep(1000000);
    }
}

int main9(void){

    pthread_t p_thread[2];
    int a = 1;
    int b = 2;
    pthread_create(&p_thread[0], NULL, thread1, (void *)&a);
    pthread_create(&p_thread[1], NULL, thread2, (void *)&b);

    int status;
    pthread_join(p_thread[0], (void **)&status);
    pthread_join(p_thread[1], (void **)&status);

    return 0;
}

void startscrew(int argc, char **argv)
{
	wiringPiSetup();
	
	// pinMode(25, OUTPUT);	// 1st motor
	// digitalWrite(25, HIGH);
	
	// pinMode(26, OUTPUT);	// 2nd motor
	// digitalWrite(26, HIGH);

	// pinMode(23, OUTPUT);	// 3rd motor
	// digitalWrite(23, HIGH);

	while(TRUE){
		char input[50];
		printf("Type \'help\' to see the command list...\n");
		fgets(input, 100, stdin);
		input[strlen(input)-1] = '\0';

		char *args[5];
		char *ptr = strtok(input, " ");
		int cnt = 0;
        while(ptr != NULL){
            args[cnt] = ptr;
            ++cnt;
            ptr = strtok(NULL, " ");
        }

		printf("args[0] > %s\n", args[0]);
		if(!strcmp(args[0], "help")){
			printf("\n\
		---------------------------------------------------------- \n\
		help : display help\n\
		spi1 : chip select 1\n\
		spi2 : chip select 2\n\
		end : terminate\n\
		tmc1 : tmc5130 on for motor 1\n\
		tmc2 : tmc5130 on for motor 2\n\
		r1 [velocity]: rotate motor 1 with velocity\n\
		r2 [velocity]: rotate motor 2 with velocity\n\
		\n\
		stop1 : stop motor 1\n\
		stop2 : stop motor 2\n\
		0 : emergency stop both motor 1 and 2\n\
		01 : emergency stop motor 1\n\
		02 : emergency stop motor 2\n\
		11 : restart motor 1\n\
		12 : restart motor 2\n\
		moveby1 [step] [velocity]: move motor 1 by step with velocity\n\
		moveby2 [step] [velocity]: move motor 1 by step with velocity\n\
		moveto1 [target] [velocity]: move motor 1 to target with velocity\n\
		moveto2 [target] [velocity]: move motor 2 to target with velocity\n\
		\n\
		read1 [address]: read spi1 address register value\n\
		read2 [address]: read spi2 address register value\n\
		write1 : \n\
		write2 : \n\
		sgresult1 : SG_RESULT for motor 1\n\
		sgresult2 : SG_RESULT for motor 2\n\
		sgt1write : \n\
		sgt2write : \n\
		readstall1 : \n\
		readstall2 : \n\
		endstall1 : end StallGuard2 stop for motor 1. call this after the motor stop.\n\
		endstall2 : end StallGuard2 stop for motor 2. call this after the motor stop.\n\
		sg1 : \n\
		sg2 : \n\
		\n\
		base : get GPIO base register\n\
		demo : run demo program\n\
		---------------------------------------------------------- \n");
		}
		// SPI ON & OFF
		else if(!strcmp(args[0], "spi1")){
			printf("chip select 1 initialize...\n");
			SPI_init();
		}
		else if(!strcmp(args[0], "spi2")){
			printf("chip select 2 initialize...\n");
			SPI_init2();
		}

		// TODO : separate 1 and 2
		else if(!strcmp(args[0], "end")){
			printf("De-initialize SPI...\n");
			SPI_deinit();
		}

		// Initialize TMC5130
		else if(!strcmp(args[0], "tmc1")){
			printf("Initialize TMC5130 for spi1...\n");
			TMC5130_init(0);
		}
		else if(!strcmp(args[0], "tmc2")){
			printf("Initialize TMC5130 for spi2...\n");
			TMC5130_init(1);
		}

		else if(!strcmp(args[0], "fastest")){
			printf("fastest motor for %d...\n", atoi(args[1]));
			TMC5130_fastest(atoi(args[1]) - 1);
			TMC5130_rotate(atoi(args[1]) - 1, 0);
		}
		else if(!strcmp(args[0], "fast")){
			printf("fast motor for %d...\n", atoi(args[1]));
			TMC5130_fast(atoi(args[1]) - 1);
			TMC5130_rotate(atoi(args[1]) - 1, 0);
		}
		else if(!strcmp(args[0], "standard")){
			printf("standard motor for %d...\n", atoi(args[1]));
			TMC5130_standard(atoi(args[1]) - 1);
			TMC5130_rotate(atoi(args[1]) - 1, 0);
		}
		else if(!strcmp(args[0], "slow")){
			printf("slow motor for %d...\n", atoi(args[1]));
			TMC5130_slow(atoi(args[1]) - 1);
			TMC5130_rotate(atoi(args[1]) - 1, 0);
		}
		else if(!strcmp(args[0], "slowest")){
			printf("slowest motor for %d...\n", atoi(args[1]));
			TMC5130_slowest(atoi(args[1]) - 1);
			TMC5130_rotate(atoi(args[1]) - 1, 0);
		}

		// Motor move with direction & velocity
		else if(!strcmp(args[0], "r1")){
			printf("Rotate spi1 with %ipps...\n", atoi(args[1]));
			TMC5130_rotate(0, atoi(args[1]));
		}
		else if(!strcmp(args[0], "r2")){
			printf("Rotate spi2 with %ipps...\n", atoi(args[1]));
			TMC5130_rotate(1, atoi(args[1]));
		}

		// Motor stop by velocity 0
		else if(!strcmp(args[0], "stop1")){
			printf("Stop motor 1...\n");
			TMC5130_rotate(0, 0);
		}
		else if(!strcmp(args[0], "stop2")){
			printf("Stop motor 2...\n");
			TMC5130_rotate(1, 0);
		}

		// TODO : Emergency stop & restart
		else if(!strcmp(args[0], "0")){
			printf("EMO stop motor 1 & 2...\n");
			TMC5130_disable_all();
		}
		else if(!strcmp(args[0], "01")){
			printf("EMO stop motor 1...\n");
			TMC5130_disable_each(0);
		}
		else if(!strcmp(args[0], "02")){
			printf("EMO stop motor 2...\n");
			TMC5130_disable_each(1);
		}
		else if(!strcmp(args[0], "1")){
			printf("EMO start motor 1 & 2...\n");
			TMC5130_enable_all();
		}
		else if(!strcmp(args[0], "11")){
			printf("EMO start motor 1...\n");
			TMC5130_enable_each(0);
		}
		else if(!strcmp(args[0], "12")){
			printf("EMO start motor 2...\n");
			TMC5130_enable_each(1);
		}



		else if(!strcmp(args[0], "read1")){
			int value = tmc5130_readInt(0, atoi(args[1]));			//read from address
			printf("Read 1 from address 0x%2x =%i[int] = %08x[hex] = %f[double])\n", atoi(args[1]), (int) value, (int) value, (double) value);
		}
		else if(!strcmp(args[0], "read2")){
			int value = tmc5130_readInt(1, atoi(args[1]));			//read from address
			printf("Read 2 from address 0x%2x =%i[int] = %08x[hex] = %f[double])\n", atoi(args[1]), (int) value, (int) value, (double) value);
		}


		else if(!strcmp(args[0], "write1")){
			tmc5130_writeInt(0, atoi(args[1]), atoi(args[2]));			//write to address
			printf("Write 1 to address 0x%2x<=%i[int] = %08x[hex] = %f[double])\n", atoi(args[1]), (int)  atoi(args[2]), (int)atoi(args[2]), (double)atoi(args[2]));
		}
		else if(!strcmp(args[0], "write2")){
			tmc5130_writeInt(1, atoi(args[1]), atoi(args[2]));			//write to address
			printf("Write 2 to address 0x%2x<=%i[int] = %08x[hex] = %f[double])\n", atoi(args[1]), (int)  atoi(args[2]), (int)atoi(args[2]), (double)atoi(args[2]));
		}



		else if(!strcmp(args[0], "sgresult1")){
			for(int i=0; i<10; ++i){
				int val = TMC5130_stallguard2SGRESULT(0);
				printf("SG_RESULT 1 : %d\n", val);
				usleep(250000);
			}
		}
		else if(!strcmp(args[0], "sgresult2")){
			for(int i=0; i<10; ++i){
				int val = TMC5130_stallguard2SGRESULT(1);
				printf("SG_RESULT 2 : %d\n", val);
				usleep(250000);
			}
		}

		/*
		// write only registers
		else if(!strcmp(args[0], "sgt1read")){
			int val = TMC5130_stallguard2SGTread(0);
			printf("SGT 1 read : %d\n", val);
		}
		else if(!strcmp(args[0], "sgt2read")){
			int val = TMC5130_stallguard2SGTread(1);
			printf("SGT 2 read : %d\n", val);
		}
		*/
		else if(!strcmp(args[0], "sgt1write")){
			TMC5130_stallguard2SGTwrite(0, (int)atoi(args[1]));
			printf("SGT 1 write : %d\n", atoi(args[1]));
		}
		else if(!strcmp(args[0], "sgt2write")){
			TMC5130_stallguard2SGTwrite(1, (int)atoi(args[1]));
			printf("SGT 2 write : %d\n", atoi(args[1]));
		}
		


		else if(!strcmp(args[0], "readstall1")){
			int val = tmc5130_readInt(0,  TMC5130_SWMODE);
			printf("sg_stop 1 : %d\n", val);
		}
		else if(!strcmp(args[0], "readstall2")){
			int val = tmc5130_readInt(1,  TMC5130_SWMODE);
			printf("sg_stop 2 : %d\n", val);
		}

		else if(!strcmp(args[0], "endstall1")){
			tmc5130_writeInt(0,  TMC5130_SWMODE, 0);
			sleep(0.5);
			tmc5130_writeInt(0, TMC5130_SWMODE, 0x400);
			// TMC5130_init(0);
			printf("disabled sg_stop 1 \n");
		}
		else if(!strcmp(args[0], "endstall2")){
			tmc5130_writeInt(1,  TMC5130_SWMODE, 0);
			sleep(0.5);
			tmc5130_writeInt(1, TMC5130_SWMODE, 0x400);
			// TMC5130_init(1);
			printf("disabled sg_stop 2 \n");
		}



		else if(!strcmp(args[0], "sg1")){
			int val = TMC5130_stallguard2StallGuard(0);
			printf("SG_RESULT 1 : %d\n", val);
		}
		else if(!strcmp(args[0], "sg2")){
			int val = TMC5130_stallguard2StallGuard(1);
			printf("SG_RESULT 2 : %d\n", val);
		}



		else if(!strcmp(args[0], "moveby1")){
			printf("Move 1 to by %d steps with velocity %ipps..\n", atoi(args[1]), atoi(args[2]));
			TMC5130_rotate(0, 0);									// stop motor
			tmc5130_writeInt(0, TMC5130_VMAX, abs(atoi(args[2])));	// set velocity
			TMC5130_moveBy(0, atoi(args[1]));						// set target
		}
		else if(!strcmp(args[0], "moveby2")){
			printf("Move 2 to by %d steps with velocity %ipps..\n", atoi(args[1]), atoi(args[2]));
			TMC5130_rotate(1, 0);									// stop motor
			tmc5130_writeInt(1, TMC5130_VMAX, abs(atoi(args[2])));	// set velocity
			TMC5130_moveBy(1, atoi(args[1]));						// set target
		}

		else if(!strcmp(args[0], "moveto1")){
			printf("Move 1 to absolute position %d with velocity %ipps..\n", atoi(args[1]), atoi(args[2]));
			TMC5130_rotate(0, 0);									// stop motor
			tmc5130_writeInt(0, TMC5130_VMAX, abs(atoi(args[2])));	// set velocity
			TMC5130_moveTo(0, atoi(args[1]));						// set target
		}
		else if(!strcmp(args[0], "moveto2")){
			printf("Move 2 to absolute position %d with velocity %ipps..\n", atoi(args[1]), atoi(args[2]));
			TMC5130_rotate(1, 0);									// stop motor
			tmc5130_writeInt(1, TMC5130_VMAX, abs(atoi(args[2])));	// set velocity
			TMC5130_moveTo(1, atoi(args[1]));						// set target
		}



		else if(!strcmp(args[0], "base")){
			uint32_t* spi_regbase = SPI_regbase();
			printf("GPIO register base : %d %08x\n", *spi_regbase, *spi_regbase);
		}


		
		// TODO
		//* teaching point
		// else if(!strcmp(args[0], "set1")){
		// 	printf("Stop motor 2...\n");
		// 	TMC5130_rotate(1, 0);
		// }
		// else if(!strcmp(args[0], "set2")){
		// 	printf("Stop motor 2...\n");
		// 	TMC5130_rotate(1, 0);
		// }

		//* demo program
		else if(!strcmp(args[0], "demo")){
			printf("Running demo program...\n");
			TMC5130_init(0);
		}


		else{
			printf("Unknown command. Try another one...\n");
		}

	}

}
