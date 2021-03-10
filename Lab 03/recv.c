#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(void)
{
	msg r;
	int i, res, corrupt_messages = 0, total_messages = 0;
	char crc2;
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	
	res = recv_message(&r);
	if (res < 0) {
		return -1;
	}
	res = send_message(&r);
	if (res < 0) {
		return -1;
	}
int k = 0;
	while (1) {
		/* wait for message */
		res = recv_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}
		/* send dummy ACK */
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
		if (r.len == 0) {
			if (r.payload[0] == '*') {
				printf("\n\nAm terminat de primit\n\n");
				break;
			}
		}
		total_messages++;
		crc2 = 0;
		for(i = 0; i < r.len; i++) {
			crc2 = crc2 ^ r.payload[i];
		}
		printf("received crc%d = %c must be %c\n", k++, crc2, r.crc);
		if(crc2 != r.crc) {
			corrupt_messages++;
			printf("and it isnt\n");
		}
	}

	printf("[RECEIVER] Finished receiving..\n[RECEIVER] Received %d corrupted messages out of %d total messages\n", corrupt_messages, total_messages);
	return 0;
}
