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
void PrintETH(unsigned char* , int);
void PrintIPH(unsigned char* , int);
void PrintBUF(unsigned char* , int); //for debugging to help visualize the packet

int main(){
    int socketfd;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *) malloc(65536);  // Allocate buffer to hold the packet, buffer currently points at first address of allocated memory

    socketfd = socket(AF_PACKET , SOCK_RAW , htons(ETH_P_ALL));

    if(socketfd < 0){
        perror("Socket error");
        return 1;
    }

    int count = 0;
    int data_size, saddr_size;
    while(count < 5){
        saddr_size = sizeof(saddr);
        data_size = recvfrom(socketfd, buffer, 65536, 0, &saddr, &saddr_size); //packet stored at where buffer is pointing
        printf("Captured Packet Size: %d bytes\n", data_size);
        count++; 
        PrintBUF(buffer, data_size);
        memset(buffer, 0, 65536);
        ProcessPacket(buffer, data_size);
    }

    close(socketfd);
    return 0;
}

void ProcessPacket(unsigned char* buffer, int data_size){
    struct iphdr *iph = (struct iphdr *) (buffer + 14); //+14 because I am recieving packets with the L2 Ethernet header, Ethernet header is 14 bytes
    switch(iph->protocol){                              //I want the L3 IP Header 
        case 6:
        PrintTCP(buffer, data_size);
    }                     
}

void PrintTCP(unsigned char* buffer, int data_size){
    printf("TCP Packet\n");
    //PrintETH(buffer, data_size);

}

void PrintUDP(unsigned char* buffer, int data_size){
    printf("UDP Packet\n");

}

/*
void PrintETH(unsigned char* buffer, int data_size){
    struct ethhdr *eth = (struct ethhdr *) buffer;

    printf("*********************ETH HEADER***********************\n");
    printf("|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5]);
    printf("|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5]);
    printf("|-Protocol : %u\n", eth->h_proto);
    printf("******************************************************\n");
}
*/

void PrintIPH(unsigned char* buffer, int data_size){
    printf("TCP Packet\n");

}

void PrintBUF(unsigned char* buffer, int size) {
    printf("Buffer content (hex):\n");
    for(int i = 0; i < size; i++) {
        printf("%.2X ", buffer[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\nEnd of buffer content\n");
}
