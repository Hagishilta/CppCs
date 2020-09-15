#include "SPI.h"
#include "TMC5160.h"
#include <math.h>
#include <wiringPi.h>

//* WRITE REGISTERS
void tmc5130_writeDatagram(unsigned char motor, unsigned char address, unsigned char x1, unsigned char x2, unsigned char x3, unsigned char x4)
{ // write TMC5130 register, where x1 is the highest and x4 the lowest byte
	unsigned char buf[] = {address | 0x80, x1,x2,x3,x4};
	SPIWrapper(motor, buf);
}

void tmc5130_writeInt(unsigned char motor, unsigned char address, int value)
{ // write TMC5130 register
	
	switch(motor)
	{
		case 0:
			// digitalWrite(25, LOW);
			tmc5130_writeDatagram(motor, address, 0xFF & (value>>24), 0xFF & (value>>16), 0xFF & (value>>8), 0xFF & (value>>0));
			// digitalWrite(25, HIGH);
		break;
		case 1:
			// digitalWrite(26, LOW);
			tmc5130_writeDatagram(motor, address, 0xFF & (value>>24), 0xFF & (value>>16), 0xFF & (value>>8), 0xFF & (value>>0));
			// digitalWrite(26, HIGH);
		break;
		// case 2:
		// 	// digitalWrite(23, LOW);
		// 	tmc5130_writeDatagram(motor, address, 0xFF & (value>>24), 0xFF & (value>>16), 0xFF & (value>>8), 0xFF & (value>>0));
		// 	// digitalWrite(23, HIGH);
		// break;
	}
		
}

int tmc5130_readInt(unsigned char motor, unsigned char address)
{ // read TMC5130 register
	unsigned char buf[] = {address & 0x7F, 0,0,0,0};
	SPIWrapper(motor, buf);

	int value = buf[1];
	value <<= 8;
	value |= buf[2];
	value <<= 8;
	value |= buf[3];
	value <<= 8;
	value |= buf[4];

	return value;
}



//* VELOCITY MODE
void TMC5130_rotate             (unsigned char motor, int velocity)
{ // rotates the motor with a defined velocity, sign defines direction

  // set absolute velocity, independant from direction
    tmc5130_writeInt(motor, TMC5130_VMAX, (velocity>=0) ? velocity : -velocity);

  // signdedness defines velocity mode direction bit in rampmode register
    tmc5130_writeInt(motor, TMC5130_RAMPMODE, (velocity >= 0) ? 1 : 2);

}

void TMC5130_right              (unsigned char motor,   int velocity)
{ // rotate motor with positive step count
  TMC5130_rotate(motor, velocity);
};

void TMC5130_left               (unsigned char motor,   int velocity)
{ // rotate motor with negative step count
  TMC5130_rotate(motor, -velocity);
};



//* POSITION MODE
void TMC5130_moveTo             (unsigned char motor,   int position)
{ // moves the motor to a certain position, make sure to set acceleration and velocity values upfront

  // set position
  tmc5130_writeInt(motor, TMC5130_XTARGET, position);

  // change to positioning mode
  tmc5130_writeInt(motor, TMC5130_RAMPMODE, 0);
};

void TMC5130_moveBy             (unsigned char motor, int steps)
{ // determine actual position and add numbers of ticks and than command to move to this position

  // determine actual position and add numbers of ticks to move
  steps = tmc5130_readInt(motor, TMC5130_XACTUAL) + steps;

  // move to position
  TMC5130_moveTo(motor, steps);
};



//* INITIALIZE
void TMC5130_init(unsigned char motor)
{
	// General Configuration Register
	tmc5130_writeInt(motor, TMC5130_GCONF, 4);  // 0, Enable stealthCHop (en_pwm_mode)
	

	// Velocity Dependent Driver Feature Control Register
	tmc5130_writeInt(motor, TMC5130_TPWMTHRS, 500);	// 19
	// tmc5130_writeDatagram(motor, TMC5130_IHOLD_IRUN, 0, 7, 5, 2);  // 0.35Apeak	/ 0.25A rms [NO]
	tmc5130_writeDatagram(motor, TMC5130_IHOLD_IRUN, 0, 7, 16, 2);  // 16, 0.00Apeak	/ 0.71A rms


	// Motor Driver Register
	tmc5130_writeInt(motor, TMC5130_PWMCONF, 0x000401C8);	// 112, Voltage PWM mode chopper configuration
	tmc5130_writeInt(motor, TMC5130_CHOPCONF, 0x000100C3);	// 108, chopper and driver configuration 


	//Reset position
	tmc5130_writeInt(motor, TMC5130_RAMPMODE, TMC5130_MODE_POSITION);	// 32
	tmc5130_writeInt(motor, TMC5130_XTARGET, 0);						// 45
	tmc5130_writeInt(motor, TMC5130_XACTUAL, 0);						// 33

	TMC5130_fastest(motor);
	//Standard values for speed and acceleration
	// tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	// tmc5130_writeInt(motor, TMC5130_A1, 30000);		// 36
	// tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	// tmc5130_writeInt(motor, TMC5130_AMAX, 10000);	// 38
	// tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39
	// tmc5130_writeInt(motor, TMC5130_DMAX, 10000);	// 40
	// tmc5130_writeInt(motor, TMC5130_D1, 30000);		// 42
	// tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43


	tmc5130_writeInt(motor, TMC5130_SWMODE, 0x400);	// 52, enable stallguard2
	tmc5130_writeInt(motor, TMC5130_TCOOLTHRS, 100);	// TCOOLTHRS > TSTEP (= 1048575 = 0xfffff)
	tmc5130_writeInt(motor, TMC5130_THIGH, 2000000);
}



//* CUSTOM FUNCTION
void TMC5130_fastest(unsigned char motor)
{
    tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43
	tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39

	tmc5130_writeInt(motor, TMC5130_A1, 90000);		// 36
	tmc5130_writeInt(motor, TMC5130_AMAX, 40000);	// 38
	tmc5130_writeInt(motor, TMC5130_DMAX, 50000);	// 40
	tmc5130_writeInt(motor, TMC5130_D1, 100000);	// 42
}

void TMC5130_fast(unsigned char motor)
{
    tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43
	tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39

	tmc5130_writeInt(motor, TMC5130_A1, 60000);		// 36
	tmc5130_writeInt(motor, TMC5130_AMAX, 20000);	// 38
	tmc5130_writeInt(motor, TMC5130_DMAX, 30000);	// 40
	tmc5130_writeInt(motor, TMC5130_D1, 70000);		// 42
}

void TMC5130_standard(unsigned char motor)
{
    tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43
	tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39

	tmc5130_writeInt(motor, TMC5130_A1, 30000);		// 36
	tmc5130_writeInt(motor, TMC5130_AMAX, 10000);	// 38
	tmc5130_writeInt(motor, TMC5130_DMAX, 10000);	// 40
	tmc5130_writeInt(motor, TMC5130_D1, 30000);		// 42
}

void TMC5130_slow(unsigned char motor)
{
    tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43
	tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39

	tmc5130_writeInt(motor, TMC5130_A1, 10000);		// 36
	tmc5130_writeInt(motor, TMC5130_AMAX, 1000);	// 38
	tmc5130_writeInt(motor, TMC5130_DMAX, 1000);	// 40
	tmc5130_writeInt(motor, TMC5130_D1, 10000);		// 42
}

void TMC5130_slowest(unsigned char motor)
{
    tmc5130_writeInt(motor, TMC5130_VSTART, 1);		// 35
	tmc5130_writeInt(motor, TMC5130_VSTOP, 10);		// 43
	tmc5130_writeInt(motor, TMC5130_V1, 50000);		// 37
	tmc5130_writeInt(motor, TMC5130_VMAX, 200000);	// 39

	tmc5130_writeInt(motor, TMC5130_A1, 300);		// 36
	tmc5130_writeInt(motor, TMC5130_AMAX, 100);	// 38
	tmc5130_writeInt(motor, TMC5130_DMAX, 100);	// 40
	tmc5130_writeInt(motor, TMC5130_D1, 300);		// 42
}



void TMC5130_disable_all()
{
	int curval1 = tmc5130_readInt(0, TMC5130_GCONF);
	tmc5130_writeInt(0, TMC5130_GCONF, curval1 | (int)pow(2.0, 15.0));
	int curval2 = tmc5130_readInt(1, TMC5130_GCONF);
	tmc5130_writeInt(1, TMC5130_GCONF, curval2 | (int)pow(2.0, 15.0));
}
void TMC5130_disable_each(unsigned char motor)
{
	int curval = tmc5130_readInt(motor, TMC5130_GCONF);
	tmc5130_writeInt(motor, TMC5130_GCONF, curval | (int)pow(2.0, 15.0));
}

void TMC5130_enable_all()
{
	int curval1 = tmc5130_readInt(0, TMC5130_GCONF);
	tmc5130_writeInt(0, TMC5130_GCONF, curval1 & ((int)pow(2.0, 32.0) - 1) - (int)pow(2.0, 15.0));
	int curval2 = tmc5130_readInt(1, TMC5130_GCONF);
	tmc5130_writeInt(1, TMC5130_GCONF, curval2 & ((int)pow(2.0, 32.0) - 1) - (int)pow(2.0, 15.0));
}
void TMC5130_enable_each(unsigned char motor)
{
	int curval = tmc5130_readInt(motor, TMC5130_GCONF);
	tmc5130_writeInt(motor, TMC5130_GCONF, curval & ((int)pow(2.0, 32.0) - 1) - (int)pow(2.0, 15.0));
}



// write only registers
// int TMC5130_stallguard2SGTread(unsigned char motor){
// 	int thresholdVal = tmc5130_readInt(motor, TMC5130_COOLCONF);
// 	return (thresholdVal>>16) & 127;
// }

void TMC5130_stallguard2SGTwrite(unsigned char motor, int value){
	int newVal = value << 16;
	tmc5130_writeInt(motor, TMC5130_COOLCONF, newVal);
}



int TMC5130_stallguard2sfilt(){
	
}

int TMC5130_stallguard2SGRESULT(unsigned char motor){
	int sg_result = tmc5130_readInt(motor, TMC5130_DRVSTATUS);
	return sg_result & 1023;
}

int TMC5130_stallguard2StallGuard(unsigned char motor){
	int sg_result = tmc5130_readInt(motor, TMC5130_DRVSTATUS);
	return (sg_result>>24) & 1;
}

int TMC5130_stallguard2sgstop(){
	
}


//* STANDALONE OPERATION - unkomment for standalone operation
// int main(int argc, char **argv)
// {// initialize SPI & TMS5130 and turn motor
// 	SPI_init();
// 	TMC5130_init(0);
// 	TMC5130_right(0,100000);
// 	SPI_deinit();
// 	return 0;
// }
/**/
