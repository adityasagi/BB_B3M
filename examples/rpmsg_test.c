
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


#define BUF_SIZE 100


void b3m_command(int fd, uint8_t *pkt) {

	char read_buffer[BUF_SIZE];
	int i;
	int ret_in;

	for(i=0; i<BUF_SIZE; i++)
		read_buffer[i]=0;

	write(fd, pkt, pkt[0]);
	ret_in = read(fd, read_buffer, BUF_SIZE);
	if(ret_in < 0) {
		printf("Unable to read the file.\n");
	} else {
		printf("%0x %0x %0x %0x %0x\n", read_buffer[0], read_buffer[1], read_buffer[2], read_buffer[3], read_buffer[4]); 
		printf("%d\n", ret_in);
	}
	for(i=0; i<BUF_SIZE; i++)
		read_buffer[i]=0;

	return;
}

int main(int argc, char* argv[]) {


	int fd;
	char data_out[]= "tesretring\n";
	int i;

	// Set to FREE MODE
	uint8_t pkt1[]={0x08, 0x04, 0x00, 0x00, 0x02, 0x28, 0x01, 0x37};

	// Set to VELOCITY CONTROL MODE
	uint8_t pkt2[]={0x08, 0x04, 0x00, 0x00, 0x06, 0x28, 0x01, 0x3B};

	// Set the GAIN PRESETS
	uint8_t pkt3[]={0x08, 0x04, 0x00, 0x00, 0x01, 0x5c, 0x01, 0x6A};

	// Set to NORMAL MODE
	uint8_t pkt4[]={0x08, 0x04, 0x00, 0x00, 0x04, 0x28, 0x01, 0x39};

	// Set the VELOCITY
	uint8_t pkt5[]={0x09, 0x04, 0x00, 0x00, 0x10, 0x27, 0x30, 0x01, 0x75};

	// Set the VELOCITY (set to 0 velocity)
	uint8_t pkt6[]={0x09, 0x04, 0x00, 0x00, 0x00, 0x00, 0x30, 0x01, 0x3E};


	fd = open("/dev/rpmsg_pru30", O_RDWR);
	if (fd < 0){
		printf("Unable to open file. \n");
		exit(-1);
	}

	b3m_command(fd, pkt1);
	usleep(1000);
	b3m_command(fd, pkt2);
	usleep(1000);
	b3m_command(fd, pkt3);
	usleep(1000);
	b3m_command(fd, pkt4);
	usleep(1000);

	for(i=0; i< 5; i++){
	b3m_command(fd, pkt5);
	sleep(5);
	b3m_command(fd, pkt6);
	sleep(5);
	}

	close(fd);

	return 0;

}
