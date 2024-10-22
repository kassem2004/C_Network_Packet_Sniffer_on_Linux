#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>  

int main(){
    int socketfd;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *) malloc(65536);  // Allocate buffer to hold the packet

    socketfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(socketfd < 0){
        perror("Socket error");
        return 1;
    }

    int count = 0;
    int data_size, saddr_size;
    while(count < 10){
        saddr_size = sizeof(saddr);
        data_size = recvfrom(socketfd, buffer, 65536, 0, &saddr, &saddr_size);
        printf("Captured Packet Size: %d bytes\n", data_size);
        count++; 
    }

    close(socketfd);
    return 0;
}