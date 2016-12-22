#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <time.h>

static const char *device1 = "/dev//spidev1.0" // motor slave
static const char *device2 = "/dev//spidev2.0" // scanner slave
static uint32_t buff;			//transfer buffer
static uint8_t bits = 1;		//number of bits  transfered at a time
static uint32_t speed = 1000000; //clock frequency in Hz set to 1MHz, same as the Atmegas
static uint16_t delay;			//will be 0 in our case

//receives a single integer from the sensor via SPI. Used to report distance and direction of closest obejct
uint8_t receiveScan(int fd)
{
	int status;
	uint8_t tx = 1; //filler value
	uint8_rx = 1;
	struct spi_ioc_transfer tr ={
		.tx_bu = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = sizeof(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	status = ioctl(fd, SPI_IOC_MESSAGE(1_, &tr); //sending and receiving
	return rx;
}

//sending a value to the car via SPI. OIs a direction, but can potentially be a PWM value
void sendDir(int fd, int t)
{
	int status;
	uint8_t tx = t;
	uint8_t rx = 1; //filler value
	struct spi_ioc_transfer tr ={
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = sizeof(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	status = ioctl(fd, SPI_IOC_MESSAGE(1_, &tr);
}

int main()
{
	int fd;						//device address 
	uint8_t receivedValue;
	uint8_t direction;
	uint8_t sendingSpeed[2];
	int velocity;
	
	while(1)
	{
		//getting direction to closest object from sensors
		fd = open(device2, O_RDWR);
		receivedValue = receiveScan(fd);
		close(fd);
		direction = receivedValue;
		usleep(50); //sleep to allow sensor chip to prepare next set of data corresponding to distance
		
		//getting distance to closest obejct from sensors
		fd = open(device2, O_RDWR);
		receivedValue = receiveScan(fd);
		close(fd);
		
		//doing something with the values
		//in our case we convert proximity to a PWM value to be used as speed.
		//The closer the object, the higher the PWM.
		//In addition, we set the new distance to be the opposite of the direction of the closest object
		//In our case direction 0 =forward, direction 1 =backward
		velocity = (int) (1000-receivedValue)/(1000*25);
		switch (direction){
			case 1: //closest object is at the front, move back
				direction = 2;
				break;
			case 2: //closest object in the rear, move forward
				direction = 1;
				break;
			default: //default move forward
				direction = 1;
		}

		//sending the values. Velocity and PWM h-bridges were scoped out last minute,
		//and so velocity is not sent
		int sendingSpeed = direction; 
		fd = open(device1, O_RDWR);
		sendDir(fd, sendingSpeed);
		close(fd);
		
		usleep(999999); //sleep for a second
	}
	return 1
}	
