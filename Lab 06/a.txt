/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int sockid = socket(PF_INET, SOCK_DGRAM, 0);

	if (sockid == -1) {
		printf("NU E BUN");
		return -1;
	}	
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/

	inet_aton(argv[1], &(to_station.sin_addr));
	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(atoi(argv[2]));
	
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/	
	int br = read(fd, buf, BUFLEN);
	while(br) {
		printf("%s", buf);
		sendto(sockid, buf, br, 0, (struct sockaddr *) &to_station, sizeof(to_station));
		br = read(fd, buf, BUFLEN);
	}


	/*Inchidere socket*/
	close(sockid);
	
	/*Inchidere fisier*/
	close(fd);
	return 0;
}
