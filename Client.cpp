#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <iostream>

using namespace std;
  
int main(int argc, char const *argv[])
{
    //variables
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    string filename;
    char* msg;
    char buffer[1024] = {0};
    int PORT;
    
    //prompt for port number
    cout << "Please enter port number: ";
    cin >> PORT;

    cout << "Enter file name: ";
    cin >> filename;
    msg = (char*)filename.c_str();

    sock = socket(AF_INET, SOCK_STREAM, 0);
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock , msg , strlen(msg) , 0 );
    printf("Filename sent to server\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}