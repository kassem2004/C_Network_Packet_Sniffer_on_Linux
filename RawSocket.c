#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <netinet/udp.h>	
#include <netinet/tcp.h>	
#include <netinet/ip.h>

void ProcessPacket(unsigned char* , int);
void PrintUDP(unsigned char* , int);
void PrintTCP(unsigned char* , int);

int main(){
    int socketfd;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *) malloc(65536);  // Allocate buffer to hold the packet

    socketfd = socket(AF_PACKET , SOCK_RAW , htons(ETH_P_ALL));

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
        ProcessPacket(buffer, data_size);
    }

    close(socketfd);
    return 0;
}

void ProcessPacket(unsigned char* buffer, int data_size){
    struct iphdr *iph = (struct iphdr *) buffer;
    switch(iph->protocol){
        case 1:
            printf("ICMP Packet");
			break;
		case 2:
			printf("IGMP Packet");
			break;
        case 6:
            printf("TCP Packet");
            break;
        case 17:
            printf("UDP Packet");
            break;
    }
}