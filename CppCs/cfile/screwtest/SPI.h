#ifndef SPI_H_
	#define SPI_H_

	void SPIWrapper(unsigned char motor, unsigned char *buf);
	void SPI_init();
	void SPI_init2();
	void SPI_deinit();
	unsigned int* SPI_regbase();

#endif
