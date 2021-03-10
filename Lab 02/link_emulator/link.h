#ifndef LINK
#define LINK
#include "lib.h"

#define MAX_VALUE 1396

typedef struct {
  msg* m;
  unsigned long long finish_time;
} msg_in_flight;

typedef struct {
	int type;
	char payload[MAX_VALUE];
} my_msg;

#endif
