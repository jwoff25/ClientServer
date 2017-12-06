#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#define PORT 8080

using namespace std;

void *handleMessage(char[]);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int num_threads = 64;
    int thread_count = 0;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    pthread_t *threads;
    threads = (pthread_t*)calloc(num_threads,sizeof(pthread_t));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("Text file: %s\n",buffer );
    //char* copy = strdup(buffer);
    // pthread_create(&threads[thread_count++], NULL, &handleMessage, (void*)buffer);
    // for (int i = 0; i < num_threads; i++){
	// 	pthread_join(threads[i],NULL);
    // }
    string line;
    ifstream file (buffer);
    while (getline(file, line)){
        cout << line << endl;
    }
    file.close();
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}

void *handleMessage(char c[]){
    printf("%s", c);
}