// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result, *p;

	// TODO: set hints
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	// TODO: get addresses
	int r =  getaddrinfo(name, "80", &hints, &result);
	if (r < 0) {
		printf("getaddrinfo: %s\n", gai_strerror(r));
		return;
	}
	// TODO: iterate through addresses and print them
	struct sockaddr_in *aux;
	struct sockaddr_in6 *aux6;
	for (p = result; p != NULL; p = p->ai_next) {
		printf("New result:\n");
		if (p->ai_family == AF_INET) {
			aux = (struct sockaddr_in *) p->ai_addr;
			char *dst = (char *) malloc(sizeof(INET_ADDRSTRLEN));
			inet_ntop(AF_INET, &(aux->sin_addr), dst, sizeof(struct sockaddr_in));
			printf("IPv4 Address: %s\n", dst);
			printf("flags: %d\n", p->ai_flags);
			printf("ai_socktype: %d\n", p->ai_socktype);
			printf("ai_socktype: %d\n", p->ai_socktype);
			printf("ai_canonname: %s\n", p->ai_canonname);
			free(dst);
		}
		if (p->ai_family == AF_INET6) {
			aux6 = (struct sockaddr_in6 *) p->ai_addr;
			char *dst = (char *) malloc(INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, &(aux6->sin6_addr), dst, sizeof(struct sockaddr_in6));
			printf("IPv6 Address: %s\n", dst);
			printf("flags: %d\n", p->ai_flags);
			printf("ai_socktype: %d\n", p->ai_socktype);
			printf("ai_socktype: %d\n", p->ai_socktype);
			printf("ai_canonname: %s\n", p->ai_canonname);
			free(dst);
		}
		printf("\n");
    }

	// TODO: free allocated data
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

    // TODO: fill in address data
    memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	struct in_addr aux;
	inet_aton(ip, &aux);
	addr.sin_addr = aux;
	// TODO: get name and service
	getnameinfo((struct sockaddr *) &addr, sizeof(struct sockaddr), host, sizeof(host), service, sizeof(service), 0);
	// TODO: print name and service
	printf("Result:\n name: %s\n service: %s\n", host, service);
}	

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
