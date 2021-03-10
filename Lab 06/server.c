/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int sockid = socket(PF_INET, SOCK_DGRAM, 0);

	if (sockid == -1) {
		printf("NU E BUN");
		return -1;
	}
	
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */

	struct in_addr address;
	address.s_addr = INADDR_ANY;

	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons(atoi(argv[1]));
	my_sockaddr.sin_addr = address;
	
	/* Legare proprietati de socket */
	int rs = bind(sockid, (struct sockaddr *) &my_sockaddr, sizeof(my_sockaddr));

	if (rs < 0) {
		printf("NU E BUN");
		return -1;
	}

	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/
	socklen_t addrlen;
	int br = recvfrom(sockid, buf, BUFLEN, 0, (struct sockaddr *) &from_station, &addrlen);
	printf("%d\n", br);
	while(br > 0) {
		write(fd, buf, br);
		printf("%s", buf);
		br = recvfrom(sockid, buf, BUFLEN, 0, (struct sockaddr *) &from_station, &addrlen);
	}


	/*Inchidere socket*/	
	close(sockid);
	/*Inchidere fisier*/
	close(fd);
	return 0;
}
