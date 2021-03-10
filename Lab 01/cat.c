#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char *argv[]) {	
	int fd = open(argv[1], O_RDONLY);
	char *buffer = (char *) calloc(1024, sizeof(char));
	int a = read(fd, buffer, 1024);
	//printf("%s", buffer);
	int k = 0;
	char **tac = (char *) malloc(100 * sizeof(char *));
	char *token = strtok(buffer, "\n");
	while (token != NULL) {
		tac[k++] = token;
		token = strtok(0, "\n");	
	}
	k--;
	while (k >= 0) {
		printf("%s\n", tac[k]);
		k--;
	}
	return 0;
}

