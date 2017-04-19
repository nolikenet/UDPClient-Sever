#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/fcntl.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8888


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    //udp socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("failed to create socket");
    }

   /* int nonBlocking = 1;
    if ( fcntl( s,
            F_SETFL,
            O_NONBLOCK,
            nonBlocking ) == -1 )
    {
        die("Failed to set non-blocking");
    } */
    //zero out the structure
    memset((char*) &si_me, 0, sizeof(si_me));

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    //bind socket to port
    if ( bind (s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
    {
        die("failed to bind socket");
    }

    //keep listening for data
    while(1)
    {
        printf("...Waiting for data \n");
        fflush(stdout);
        //blocking call, receive data
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("failed to recv from");
        }


        printf("\n Received packet from %s: %d \n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s \n", buf);

        //reply to client with some data
        if (sendto (s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("failed to sendto() \n");
        }

        memset(buf, '\0', BUFLEN);
    }
     
    return 0;
}
