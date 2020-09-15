#include <stdio.h>
#include <stdlib.h>
#include <bcm2835.h>
#include <stdint.h>
#include "SPI.h"
// #include <wiringPi.h>

void SPIWrapper(unsigned char motor, unsigned char *buf)
{ // the SPI multiplexis the SPI communication to different devices, writes reply to buf
	switch(motor)
	{
		case 0:
			// digitalWrite(25, LOW);
			bcm2835_spi_chipSelect(BCM2835_SPI_CS0); 				// assign chip select
			bcm2835_spi_transfernb((char *) buf, (char *) buf, 5); 	// send data
			// digitalWrite(25, HIGH);
		break;
		case 1:
			// digitalWrite(26, LOW);
			bcm2835_spi_chipSelect(BCM2835_SPI_CS1); 				// assign chip select
			bcm2835_spi_transfernb((char *) buf, (char *) buf, 5); 	// send data
			// digitalWrite(26, HIGH);
		break;
		case 2:
			// digitalWrite(23, LOW);
			bcm2835_spi_chipSelect(BCM2835_SPI_CS0); 				// assign chip select
			bcm2835_spi_transfernb((char *) buf, (char *) buf, 5); 	// send data
			// digitalWrite(23, HIGH);
		break;
		

		/*
		case 0:	// TMC5130-BOB chip select 0
			// printf("------------ chip select 0");
			bcm2835_spi_chipSelect(BCM2835_SPI_CS0); 				// assign chip select
			bcm2835_spi_transfernb((char *) buf, (char *) buf, 5); 	// send data
		break;

		case 1:	// TMC5130-BOB chip select 1
			// printf("------------ chip select 1");
			bcm2835_spi_chipSelect(BCM2835_SPI_CS1); 				// assign chip select
			bcm2835_spi_transfernb((char *) buf, (char *) buf, 5); 	// send data
		break;
		*/
		default:
			printf("Unknown chip");
		break;
	}
}

void SPI_init()
{ // set up SPI driver for Trinamic products
	if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return;
    }
 
    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return;
    }
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 	// The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); 				// The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW); 	// chip select 0
}

void SPI_init2()
{ // set up SPI driver for Trinamic products
	if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return;
    }
 
    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return;
    }
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); 	// The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); 				// The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256); // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);	// chip select 1
}


void SPI_deinit()
{ // release the SPI driver
	bcm2835_spi_end();
	bcm2835_close();
}


uint32_t* SPI_regbase(){
	return bcm2835_regbase(BCM2835_REGBASE_GPIO);
}
