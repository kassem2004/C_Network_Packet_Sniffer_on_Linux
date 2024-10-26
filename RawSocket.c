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
void PrintUDP(unsigned char* , int, int);
void PrintTCP(unsigned char* , int, int);
void PrintETH(unsigned char* , int);
void PrintIPH(unsigned char* , int);

struct in_addr sipaddr, dipaddr;

struct TCPHeader {
    uint16_t source;          
    uint16_t dest;            
    uint32_t seq;             
    uint32_t ack_seq;         
    uint8_t  data_offset;     
    uint8_t  flags;           
    uint16_t window;          
    uint16_t check;           
    uint16_t urg_ptr;         
};

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
    while(count < 10){
        saddr_size = sizeof(saddr);
        data_size = recvfrom(socketfd, buffer, 65536, 0, &saddr, &saddr_size); //packet stored at where buffer is pointing
        count++; 
        ProcessPacket(buffer, data_size);
        memset(buffer, 0, 65536);
    }

    close(socketfd);
    return 0;
}

void ProcessPacket(unsigned char* buffer, int data_size){
    struct iphdr *iph = (struct iphdr *) (buffer + 14); //+14 because I am recieving packets with the L2 Ethernet header, Ethernet header is 14 bytes
    int iphdrlen = iph->ihl*4;                          //I want the L3 IP Header
    switch(iph->protocol){                               
        case 6:
        PrintTCP(buffer, data_size, iphdrlen);
        case 17:
        PrintUDP(buffer, data_size, iphdrlen);
    }                     
}

void PrintTCP(unsigned char* buffer, int data_size, int iphdrlen) {
    printf("TCP Packet\n");
    PrintETH(buffer, data_size);
    PrintIPH(buffer, data_size);

    struct TCPHeader *tcph = (struct TCPHeader *) (buffer + 14 + iphdrlen);
    unsigned int header_len = (tcph->data_offset >> 4) * 4;

    printf("*********************TCP HEADER***********************\n");
    printf("|-Source Port      : %u\n", ntohs(tcph->source));
    printf("|-Destination Port : %u\n", ntohs(tcph->dest));
    printf("|-Sequence Number    : %u\n", ntohl(tcph->seq));
    printf("|-Acknowledge Number : %u\n", ntohl(tcph->ack_seq));
    printf("|-Header Length      : %d DWORDS or %d BYTES\n", header_len / 4, header_len);
    printf("|-Urgent Flag          : %d\n", (tcph->flags & 0x20) >> 5);
    printf("|-Acknowledgement Flag : %d\n", (tcph->flags & 0x10) >> 4);
    printf("|-Push Flag            : %d\n", (tcph->flags & 0x08) >> 3);
    printf("|-Reset Flag           : %d\n", (tcph->flags & 0x04) >> 2);
    printf("|-Synchronise Flag     : %d\n", (tcph->flags & 0x02) >> 1);
    printf("|-Finish Flag          : %d\n", (tcph->flags & 0x01));
    printf("|-Window         : %d\n", ntohs(tcph->window));
    printf("|-Checksum       : %d\n", ntohs(tcph->check));
    printf("|-Urgent Pointer : %d\n", ntohs(tcph->urg_ptr));
    printf("******************************************************\n");
}

void PrintUDP(unsigned char* buffer, int data_size, int iphdrlen){
    printf("UDP Packet\n");
    PrintETH(buffer, data_size);
    PrintIPH(buffer, data_size);

    struct udphdr *udph = (struct udphdr *) (buffer + 14 + iphdrlen);
    printf("*********************UDP HEADER***********************\n");
    printf("|-Source Port      : %d\n" , ntohs(udph->source));
	printf("|-Destination Port : %d\n" , ntohs(udph->dest));
	printf("|-UDP Length       : %d\n" , ntohs(udph->len));
	printf("|-UDP Checksum     : %d\n" , ntohs(udph->check));
    printf("******************************************************\n");
}

void PrintETH(unsigned char* buffer, int data_size){
    struct ethhdr *eth = (struct ethhdr *) buffer;

    printf("*********************ETH HEADER***********************\n");
    printf("|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5]);
    printf("|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5]);
    printf("|-Protocol : %u\n", eth->h_proto);
    printf("******************************************************\n");
}

void PrintIPH(unsigned char* buffer, int data_size){
    struct iphdr *iph = (struct iphdr *) (buffer + 14);
    int iphdrlen = iph->tot_len;
    sipaddr.s_addr = iph->saddr;
    dipaddr.s_addr = iph->daddr;

	printf("*********************IPH HEADER***********************\n");
	printf("|-IP Version        : %d\n",(unsigned int)iph->version);
	printf("|-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
	printf("|-Type Of Service   : %d\n",(unsigned int)iph->tos);
	printf("|-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
	printf("|-Identification    : %d\n",ntohs(iph->id));
	printf("|-TTL      : %d\n",(unsigned int)iph->ttl);
	printf("|-Protocol : %d\n",(unsigned int)iph->protocol);
	printf("|-Checksum : %d\n",ntohs(iph->check));
	printf("|-Source IP        : %s\n",inet_ntoa(sipaddr));
	printf("|-Destination IP   : %s\n",inet_ntoa(dipaddr));
    printf("******************************************************\n");

}
