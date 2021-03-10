#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001


int main(int argc,char** argv){
  msg r;
  init(HOST,PORT);

  // if (recv_message(&r)<0){
  //   perror("Receive message");
  //   return -1;
  // }
  // printf("[recv] Got msg with payload: <%s>, sending ACK...\n", r.payload);

  // // Send ACK:
  // sprintf(r.payload,"%s", "ACK");
  // r.len = strlen(r.payload) + 1;
  // send_message(&r);
  // printf("[recv] ACK sent\n");



  // char filename[101];
  // if(recv_message(&r) < 1) {
  //   perror("eroare");
  //   return -1;
  // }
  // printf("filename : %s\n", r.payload + sizeof(int));

  //char *a = (char *) malloc(1400 * sizeof(char));

  if(recv_message(&r) < 1) {
    perror("eroare");
    return -1;
  }
  int length = *((int *)r.payload);
  printf("length = %d\n", length);
  printf("%s", r.payload + 4);
  length -= 1396;
// Send ACK:
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("\n\nACK sent\n\n");

  while(length >= 0) {
    if(recv_message(&r) < 1) {
      perror("eroare");
      return -1;
    }
    printf("%s", r.payload);
    length -= 1400;
    // Send ACK:
    sprintf(r.payload,"%s", "ACK");
    r.len = strlen(r.payload) + 1;
    send_message(&r);
    printf("\n\nACK sent\n\n");

  }

  return 0;
}
