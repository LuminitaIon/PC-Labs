#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char *argv[])
{
	msg t;
	int i, res;
	
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	/* printf("[SENDER]: BDP=%d\n", atoi(argv[1])); */
	
	int fd = open("in", O_RDONLY);
  	int length = lseek(fd, 0, SEEK_END);
  	printf("LENGTH = %d\n", length);
  	lseek(fd, 0, SEEK_SET);

  	int bandwidth = 10000000;
	memset(&t, 0, sizeof(msg));
	t.len = MSGSIZE;
	clock_t start = clock();
	res = send_message(&t);
	if (res < 0) {
		perror("[SENDER] Send error. Exiting.\n");
		return -1;
	}	
	/* wait for ACK */
	res = recv_message(&t);
	clock_t end = clock();
	if (res < 0) {
		perror("[SENDER] Receive error. Exiting.\n");
		return -1;
	}
	double delay = (double)(end - start) / CLOCKS_PER_SEC;
	printf("delay = %lf", delay);
	int WINDOW_SIZE = (int) (bandwidth * delay * 1000 / (sizeof(t) * 8));
	printf("window size = %d\n", WINDOW_SIZE);

	int a, j;
	char crc;
	int k = 0, ms = 500;
	clock_t start_time;
	//trimite WINDOW SIZE mesage
	for(j = 0; j < WINDOW_SIZE; j++) {
		memset(&t, 0, sizeof(msg));
		a = read(fd, t.payload, MSGSIZE);
		t.len = strlen(t.payload);
		crc = 0;
		for(i = 0; i < t.len; i++) {
			crc = crc ^ t.payload[i];
		}
		printf("Sent crc%d = %c \n", k++, crc);
		t.crc = crc;
		res = send_message(&t);
		start_time = clock();
		while(clock() < start_time + ms){
			;
		}
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
	}
	// verifica primul mesaj
	res = recv_message(&t);
	if (res < 0) {
		perror("[SENDER] Receive error. Exiting.\n");
		return -1;
	}
	memset(&t, 0, sizeof(msg));
	// Comporta ca un stop and wait
	while (a = read(fd, t.payload, MSGSIZE)) {
		/* cleanup msg */
		t.len = strlen(t.payload);
		crc = 0;
		for(i = 0; i < t.len; i++) {
			crc = crc ^ t.payload[i];
		}
		printf("Sent crc%d = %c \n", k++, crc);
		t.crc = crc;
		res = send_message(&t);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			return -1;
		}
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
		memset(&t, 0, sizeof(msg));
		
	}
	for(i = 0; i < WINDOW_SIZE - 1; i ++) {
		res = recv_message(&t);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			return -1;
		}
	}
	memset(&t, 0, sizeof(msg));
	t.len = 0;
	t.payload[0] = '*';
	res = send_message(&t);
	if (res < 0) {
		perror("[SENDER] Receive error. Exiting.\n");
		return -1;
	}
	res = recv_message(&t);
	if (res < 0) {
		perror("[SENDER] Send error. Exiting.\n");
		return -1;
	}


	printf("[SENDER] Job done, all sent.\n");
		
	return 0;
}
