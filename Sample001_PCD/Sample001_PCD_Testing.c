#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PCD_DEVICE	"/dev/PCD_DEVICE"

static uint8_t wmsg [] = "Sample001_PCD_Testing";
static uint8_t rdata [100];

int main (int argc, char ** argv) {
	int fd;
	int ret;
	int i;
	
	fd = open(PCD_DEVICE, O_RDWR);
	if (fd < 0) {
		printf("open: NG\n");
		
		return -EINVAL;
	}
	
	ret = write(fd, wmsg, sizeof(wmsg));
	
	ret = lseek(fd, 0, SEEK_SET);
	
	ret = read(fd, rdata, sizeof(rdata));
	
	for (i=0; i<sizeof(rdata); i++) {
		printf("%c", rdata[i]);
	}
	printf("\n");
	
	return 0;
}
