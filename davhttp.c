
#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */

#define RCVBUFSIZE 65536   /* Size of receive buffer */

void DieWithError(char *errorMessage);  /* Error handling function */
int GET(char* hostname, unsigned short port, char* path, char* response);



void DieWithError(char *errorMessage){
	puts(errorMessage);
	exit(-1);
}


char* resolve(char *hostname) {


    struct hostent *hp = gethostbyname(hostname);
	char *ip="aaa.bbb.ccc.ddd";

    if (hp == NULL) {
       printf("gethostbyname() failed\n");
    } else {
       printf("%s = ", hp->h_name);
       unsigned int i=0;
       while ( hp -> h_addr_list[i] != NULL) {
		  /*strcpy(ip,inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i]))); */
		  ip=inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i]));
          printf( "%s ", inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i])));
		  
          i++;
		  
       }
       printf("\n");
    }
	
	
	return ip;
}



int GET(char* hostname, unsigned short port, char* path, char *response)
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in addr; /* Echo server address */
    char *servIP="255.255.255.255";  /* Server IP address (dotted quad) */
    char *request;
    char *body;
	
	
	/* String to send to echo server */
	

    char buffer[RCVBUFSIZE];     /* Buffer for echo string */
    int stringLen;               /* Length of string to echo */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() and total bytes read */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */

    
    request = malloc(strlen(path) + 24);
	sprintf(request,"GET %s HTTP/1.1\n\n",path);
    puts(request);

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }

    servIP = resolve(hostname);

    
    

    //Return the first one;
	


    printf("%s resolved to : %s\n" , hostname , servIP);
	
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&addr, 0, sizeof(addr));     /* Zero out structure */
    addr.sin_family      = AF_INET;             /* Internet address family */
    addr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    addr.sin_port        = htons(port); /* Server port */
    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        DieWithError("connect() failed");

    stringLen = strlen(request);          /* Determine input length */

    /* Send the string, including the null terminator, to the server */
    if (send(sock, request, stringLen, 0) != stringLen)
        DieWithError("send() sent a different number of bytes than expected");

    totalBytesRcvd = 0;
    printf("Received: ");                /* Setup to print the echoed string */
    while (totalBytesRcvd < stringLen)
    {
        /* Receive up to the buffer size (minus 1 to leave space for 
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, buffer, RCVBUFSIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        
        strcpy(response + totalBytesRcvd,buffer);
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
 
    } 
    
    
    /*printf("\n");    /* Print a final linefeed */

    body = strstr(response, "\n\n");
    puts("*******");
    puts(response);
    puts("=======");
    closesocket(sock);
    WSACleanup();  /* Cleanup Winsock */

    return 0;
}


void main(int argc, char *argv[])
{
    char resp[64*1024];
	  GET("www.wikipedia.org",80,"/",resp);
    puts(resp);
}
