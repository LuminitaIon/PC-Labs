#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char url[] = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";

    if (argc != 2) {
        perror("Wrong usage\n");
        return -1;
    }
    sockfd = open_connection(argv[1], 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(url, "/api/v1/dummy", NULL, NULL, 0);
    printf("Exercitiul1\n\nMy GET request:\n %s \n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response message:\n%s\n", response);

    message = compute_post_request(url, "/api/v1/dummy", "application/x-www-form-urlencoded", 
        NULL, 0, NULL, 0);
    printf("\n\nExercitiul2\n\nMy POST request:\n %s \n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response message:\n%s\n", response);

    // Ex 1.1: GET dummy from main server
    // Ex 1.2: POST dummy and print response from main server
    // Ex 2: Login into main server
    char **login = (char **) malloc(2 * sizeof(char *));
    login[0] = (char *) calloc(20, sizeof(char));
    login[1] = (char *) calloc(20, sizeof(char));
    strcpy(login[0], "username=student");
    strcpy(login[1], "password=student");
    message = compute_post_request(url, "/api/v1/auth/login", "application/x-www-form-urlencoded", 
        login, 2, NULL, 0);
    printf("\n\nExercitiul 3\n\nMy login POST request:\n %s \n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response message:\n%s\n", response);
    // Ex 3: GET weather key from main server
    char **cookies = (char **) malloc(sizeof(char *));
    cookies[0] = (char *) calloc(300, sizeof(char));
    strcpy(cookies[0], "connect.sid=s%3ANmXboqaehq0OdklfxPp_L6zqkh0i9ZVK.cyeL5qS1Lvv70xQ0MDKs4I7UI3ovpftfPGjyf50YcUQ");
    message = compute_get_request(url, "/api/v1/weather/key", NULL, cookies, 1);
    printf("\n\nExercitiul 4\n\nMy login GET request:\n%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response message:\n%s\n", response);
    // Ex 4: GET weather data from OpenWeather API
    // Ex 5: POST weather data for verification to main server
    // Ex 6: Logout from main server
    message = compute_get_request(url, "/api/v1/auth/logout", NULL, NULL, 0);
    printf("\n\nExercitiul 5\n\nMy logout GET request:\n%s\n", message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("Response message:\n%s\n", response);

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!
    free(message);
    close_connection(sockfd);
    return 0;
}
