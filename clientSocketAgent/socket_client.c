// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8888   

int main(){
    int sock = 0, val_read;
    struct sockaddr_in serv_addr;
    char *message = "Message to be Delivered";
    char *buffer[1024] = {0};
    /*  
    //  socket is created it is just a socket initailly its not linked to any port or server initally
    //  Why so socket is like a entry point to your pipe with is entry point to any data transfer but not linked to any pipe yet just a door
    //  so you create a socket and assign initialize with the OS api
    */  
    if ((sock = socket(AF_INET, SOCK_STREAM,0)) < 0)
    {
        printf("\n SOCKET Creation ERROR \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    /*  
    //  sockadder is just a sturct with is used to send the config the sock created 
    //  so below now you are saying the socket you are communicating using AF_INET and the address you are going to communicate
    //  it will just copy the server address to the serv_addr variable
    */
    if( inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=0 ){
        printf(" ISSUE is with the server address please re config");
        return -1;
    }
    /*
    //  creates the socket connection with the port
    //  
    */
    if(connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        printf("Connection Failed");
        return -1;
    }
    /*
    //  pushs the message to the reciever the reciver can be of any application can be python too
    //  will write a server.py soon so that people can see the code and test it out
    */
    send(sock , message, strlen(message), 0);
    printf("Data message is sent");
    val_read = read( sock, buffer, 1024); 
    printf("%s\n", buffer); 
    return 0; 
}