#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

int fd_max3100 = NULL;

int init_spi_sender(void)
{
	int ret = 0;

	uint8_t mode = 0;
	uint8_t bits = 16;
	uint32_t speed = 2000000;

	fd_max3100 = open("/dev/spidev1.0", O_RDWR);

	if (fd_max3100 < 0)
	{
		printf("can't open spidev0.0\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
	{
		printf("can't set spi mode\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
	{
		printf("can't get spi mode\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		printf("can't set bits per word\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
	{
		printf("can't get bits per word\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		printf("can't set max speed hz\n");
		goto exit;
	}

	ret = ioctl(fd_max3100, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
	{
		printf("can't get max speed hz\n");
		goto exit;
	}

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);


//////init max3100/////////

unsigned char buf[2];
buf[1] = 0xe0;
buf[0] = 0x09;

	ret = write(fd_max3100, buf, 2);
	if (ret == -1)
	{
		printf("write spi error\n");
	}

	return 0;//fd_max3100;

exit:
	close(fd_max3100);
	return -1;
}

void clean_spi_sender()
{
	close(fd_max3100);
}

int send_spi_uart(char *buf, int len)
{
	unsigned int i;
	int ret = 0;
	unsigned char spibuf[2];

	if (buf == NULL)
	{
		printf("buf is NULL\n");
	}

	for(i=0; i<len; i++)
	{
		spibuf[1] = 0x80;
		spibuf[0] = buf[i];

		ret = write(fd_max3100, spibuf, 2);
		if (ret == -1)
		{
			printf("write spi error\n");
		}
		usleep(210);
	}	

	return ret;
}
