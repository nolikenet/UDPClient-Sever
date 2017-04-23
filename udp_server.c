#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <time.h>

#define BUFLEN 512 //Max length of buffer
#define PORT 8888

#define STDIN 0


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(void)
{
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    void* buf[BUFLEN];
    char* newBuf;
    time_t* seconds = malloc(sizeof(unsigned int));

    struct timeval tv;
   // fd_set readfds;
   // tv.tv_sec = 2;
   // tv.tv_usec = 500000;

   // FD_ZERO(&readfds);
   // FD_SET(STDIN, &readfds);
    time(seconds);

   // select(STDIN+1, &readfds, NULL, NULL, &tv);

  /*  if (FD_ISSET (STDIN, &readfds))
    {
        printf("A key was pressed! \n");
    }

    else 
    {
        printf("Timed output \n");
    }
    printf("Server time now is %d \n", seconds);*/

    //udp socket
    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("failed to create socket");
    }
    
    //set socket to NON-BLOCK mode
    //fcntl(s, F_SETFL, O_NONBLOCK);


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

    char* additional = (char*)malloc(sizeof(char) * 5);
    strcpy(additional, "Added");
    int sz;

    time_t timeDelay = 2;
    time_t secondsNow = *seconds; 
    //keep listening for data
    while(1)
    {
        //printf("...Waiting for data %d \n", *seconds);
        fflush(stdout);

        time(seconds);

        if (*seconds - secondsNow >= timeDelay)
        {
            printf("\n TimeStampTicks ");
            secondsNow = *seconds;
        }

        if ((recv_len = recvfrom(s, buf, sizeof(buf) , 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("failed to recv from");
        }



        printf("\n Received packet from %s: %d \n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data void: %s \n", buf);
        
        strcat((char*)buf, additional);

        printf("Data sent i NOWs %s", buf);
        sz = sendto (s, buf, sizeof(buf), 0, (struct sockaddr*) &si_other, slen);
        printf("\n size of data sent %d \n", sz);
       // reply to client with some data
        if (sz == -1)
        {
            die("failed to sendto() \n");
        } 

        memset(buf, '\0', BUFLEN);
    }
    close(s);
    //free(additional);
    //free(seconds);
    //free(&timeDelay);
    //free(&secondsNow);
     
    return 0;
}
