#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>


#include "rf.h"
#include "unp.h"

/* TODO:
 * Cannot be reused when CTRL+C kill the program
 */

int fd;

#if 0
void sigint(int signo)
{
	printf("SIGINT occor!\n");
//	close(fd);
	exit(1);
}
#endif

int main(void)
{
	int res;
	int freq;

	fd = open("/dev/spidev1.0", O_RDWR);

	if (fd < 0) {
		printf("open device error!\n");
		return -1;
	}

#if 0
	freq = 470000000;
	if (ioctl(fd, RF_IOC_SET_FREQ, &freq)) {
		printf("ioctl error!\n");
		close(fd);
		exit(1);
	}
#endif

//	signal(SIGINT, sigint);

	while (1) {
		int res;
		uint8_t buf[0x100] = "hello, world!\n";
#if 0
		sleep(1);
		printf("app write...\n");
		if ( (res = write(fd, buf, 16)) < 0 ) {
			err_msg("EINT occur!\n");
			break;
		} else if (res == 0) {
			printf("EOF!\n");
			break;
		}
#else
		int size = read(fd, buf, sizeof(buf));
		if (size > 0) 
			printf("Get: %d\n%s\n", size, buf);
		else
			break;
#endif
	}
	
	printf("close file ...\n");

	close(fd);

	return 0;
}
