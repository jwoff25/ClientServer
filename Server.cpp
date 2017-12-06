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
#include <sys/stat.h>
#define PORT 8080

using namespace std;

void *handleMessage(void*);

static int new_socket;

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int num_threads = 64;
    int thread_count = 0;
    int addrlen = sizeof(address);

    pthread_t threads[5];

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

    int threadCounter = 0;

    while (threadCounter < 5){
        cout << "Waiting for connection..." << endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_create(&threads[threadCounter++], NULL, handleMessage, NULL);
    }

    for (int i = 0; i < 5; i++){
        pthread_join(threads[i], NULL);
    }

    
    
    return 0;
}

void *handleMessage(void* v){
    char buffer[1024] = {0};
    int valread;
    int local_socket = new_socket;
    char *hello = "Hello from server";
    char *newline = "\n";
    struct stat st;

.
    valread = read( local_socket , buffer, 1024);
    printf("Text file: %s\n",buffer );
    string line;
    ifstream file (buffer);
    
    while (getline(file, line)){
        send(local_socket, line.c_str(), strlen(line.c_str()), 0);
        send(local_socket, newline, strlen(newline), 0);
    }
    stat(buffer, &st);
    int f = st.st_size;
    printf("%d", f);
    file.close();
    string test = std::to_string(f);
    send(local_socket, test.c_str(), strlen(test.c_str()), 0);
    send(local_socket, newline, strlen(newline), 0);
    send(local_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
}