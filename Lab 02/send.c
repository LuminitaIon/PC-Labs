#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"
#include "link_emulator/link.h"

#define HOST "127.0.0.1"
#define PORT 10000


int main(int argc,char** argv){
  init(HOST,PORT);
  msg t;

  // //Send dummy message:
  // printf("[send] Sending dummy...\n");
  // sprintf(t.payload,"%s", "This is a dummy.");
  // t.len = strlen(t.payload)+1;
  // send_message(&t);
  
  // // Check response:
  // if (recv_message(&t)<0){
  //   perror("Receive error ...");
  //   return -1;
  // }
  // else {
  //   printf("[send] Got reply with payload: %s\n", t.payload);
  // }

  my_msg msg2;

  char *in = "in.txt";
  char buf[1400];
  // sprintf(msg2.payload, "%s", in);
  // msg2.type = 0;
  // memcpy(t.payload, &msg2, sizeof(int) + strlen(msg2.payload) + 1);
  // t.len = sizeof(int) + strlen(msg2.payload) + 1;
  // send_message(&t);

  int fd = open(in, O_RDONLY);
  int length = lseek(fd, 0, SEEK_END);
  printf("LENGTH = %d", t.len);
  lseek(fd, 0, SEEK_SET);

  msg2.type = length;
  int a = read(fd, msg2.payload, 1396);
  memcpy(t.payload, &msg2, sizeof(int) + strlen(msg2.payload) + 1);
  t.len = sizeof(int) + strlen(msg2.payload) + 1;
  send_message(&t);

  if (recv_message(&t)<0){
    perror("Receive error ...");
    return -1;
  }
  else {
    printf("\n\n ACK RECEIVED \n\n");
  }

  length -= 1396;
  while(length >= 0) {
  	a = read(fd, buf, sizeof(buf));
  	memcpy(t.payload, buf, sizeof(buf));
  	t.len = strlen(t.payload);
  	send_message(&t);
  	length -= 1400;
  	if (recv_message(&t)<0){
    	perror("Receive error ...");
    	return -1;
  	}
  	else {
    	printf("\n\nACK RECEIVED \n\n");
  	}
  }
  return 0;
}
