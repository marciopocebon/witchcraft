//compile gcc tcpraw.c -o tcpraw

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define PCKT_LEN 8192

//IP header structure

struct ipheader {
    unsigned char       iph_ihl:5, iph_ver:4;
    unsigned char       iph_tos;
    unsigned short int  iph_len;
    unsigned short int  iph_ident;
    unsigned char       iph_flags;
    unsigned short int  iph_offset;
    unsigned char       iph_ttl;
    unsigned char       iph_protocol;
    unsigned short int  iph_chksum;
    unsigned int        iph_sourceip;
    unsigned int        iph_destip;
};

struct tcpheader {
    unsigned short int  tcph_srcport;
    unsigned short int  tcph_destport;
    unsigned int        tcph_seqnum;
    unsigned int        tcph_acknum;
    unsigned char       tcph_reversed:4, tcph_offset:4;

    unsigned int
        tcp_rest1:4,
        tcph_hlen:4,
        tcph_fin:1,
        tcph_syn:1,
        tcph_rst:1,
        tcph_psh:1,
        tcph_ack:1,
        tcph_urg:1,
        tcph_rest2:2;

    unsigned short int tcph_win;
    unsigned short int tcph_chksum;
    unsigned short int tcph_urgptr;
};

unsigned short csum(unsigned short *buf, int len){
    unsigned long sum;

    for(sum = 0; len > 0; len--){
        sum += *buf++;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (unsigned short)(~sum);
}

int main(int argc, char *argv[]){
    int sd;
    char buffer[PCKT_LEN];

    struct ipheader *ip = (struct ipheader *) buffer;
    struct tcpheader *tcp = (struct tcpheader *) (buffer + sizeof(struct ipheader));
    struct sockaddr_in sin;
    int one = 1;
    const int *val = &one;

    memset(buffer, 0, PCKT_LEN);

    sd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(sd < 0){
        perror("socket()");
        exit(1);
    }

    ip->iph_ihl = 5;
    ip->iph_ver = 4;
    ip->iph_tos = 16;
    ip->iph_len = sizeof(struct ipheader) + sizeof(struct tcpheader);
    ip->iph_ident = htons(54321);
    ip->iph_offset = 0;
    ip->iph_ttl = 64;
    ip->iph_protocol = 6;
    ip->iph_chksum = 0;
    ip->iph_sourceip = inet_addr(argv[1]);
    ip->iph_destip = inet_addr(argv[3]);
   
    tcp->tcph_srcport = htons(atoi(argv[2]));
    tcp->tcph_destport = htons(atoi(argv[4]));
    tcp->tcph_seqnum = htonl(1);
    tcp->tcph_acknum = 0;
    tcp->tcph_offset = 5;
    tcp->tcph_syn = 1;
    tcp->tcph_ack = 0;
    tcp->tcph_win = htons(32767);
    tcp->tcph_chksum = 0;
    tcp->tcph_urgptr = 0;
    
    ip->iph_chksum = csum((unsigned short *) buffer, (sizeof(struct ipheader) + sizeof(struct tcpheader)));

    if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0){
        perror("setsockopt() error");
        exit(1);
    }else{
        printf("setsockopt() is OK\n");
    }

    return 0;
}
