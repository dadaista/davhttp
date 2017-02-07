/* Simple HTTP client                                                          */
/* Written by Davide Carboni, public domain                                    */

#include <stdio.h>      /* for printf(), fprintf() */
#include <winsock.h>    /* for socket(),... */
#include <stdlib.h>     /* for exit() */

#define RCVBUFSIZE 65536   /* Size of receive buffer */

struct Response {
    char *headers;
    char *body;
};

void DieWithError(const char *errorMessage);  /* Error handling function */
int GET(const char* hostname, unsigned short port, const char* path, struct Response* response);
int openSocket(const char* hostname, unsigned short port);
char* resolve(const char *hostname);/* Resolve hostname to ip "a.b.c.d" */



void DieWithError(const char *errorMessage){
	puts(errorMessage);
	exit(-1);
}


char* resolve(const char *hostname) {
    printf("solving %s ...\n",hostname );
    struct hostent *hp = gethostbyname(hostname);
	char *ip="aaa.bbb.ccc.ddd";

    if (hp == NULL) {
       DieWithError("gethostbyname() failed\n");
    } else {
       /*printf("%s = ", hp->h_name);*/
       unsigned int i=0;
       while ( hp -> h_addr_list[i] != NULL) {
          ip=inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i]));
          i++;
       }
       /*printf("\n");*/
    }
    return ip;
}



int GET(const char* hostname, unsigned short port, const char* path, struct Response *response)
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in addr; /* Echo server address */
    char *servIP="255.255.255.255";  /* Server IP address (dotted quad) */
    char request[256];/*watch this limit*/
    
    printf("hostname:%s\n", hostname );
	printf("path:%s\n", path );
	printf("port:%i\n", port );
	/* String to send to echo server */
	

    char buffer[RCVBUFSIZE];         /* Buffer for echo string */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() and total bytes read */
    WSADATA wsaData;                 /* Structure for WinSock setup communication */

    
	sprintf(request,"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n",path,hostname);
    printf("request:%s\n",request);

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) /* Load Winsock 2.0 DLL */
    {
        fprintf(stderr, "WSAStartup() failed");
        exit(1);
    }
    printf("WSA started\n");
    servIP = resolve(hostname);

    printf("%s resolved to : %s\n" , hostname , servIP);
	
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&addr, 0, sizeof(addr));     /* Zero out structure */
    addr.sin_family      = AF_INET;             /* Internet address family */
    addr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    addr.sin_port        = htons(port); /* Server port */
    /* Establish the connection to the  server */
    printf("Connecting...");
    if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        DieWithError("connect() failed");
    puts("done");

    /* Send the string, including the null terminator, to the server */
    printf("Sending request...");
    if (send(sock, request, strlen(request), 0) != strlen(request))
        DieWithError("send() sent a different number of bytes than expected");
    puts("done");

    printf("Receiving... ");                
    /* Receive up to the buffer size (minus 1 to leave space for 
       a null terminator) bytes from the sender */
    if ((bytesRcvd = recv(sock, buffer, RCVBUFSIZE - 1, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    puts("done");
 
    
    

    strcpy(response->body, strstr(buffer, "\r\n\r\n")+4); /* +4 is to get rid of \r\n\r\n */
    char *p = strstr(buffer, "\r\n\r\n");
    (*p)='\0';
    strcpy(response->headers,buffer);

    closesocket(sock);
    WSACleanup();  /* Cleanup Winsock */

    return 0;
}


void main(int argc, char *argv[])
{
    if (argc != 4) {
        DieWithError("Usage: davhttp <host> <port> <path> \n");
    }

    char headers[64*1024];
    char body[64*1024];
    
    struct Response resp={ .headers=headers, .body=body};

    GET(argv[1],atoi(argv[2]),argv[3],&resp);
    puts("== headers ==");
    puts(resp.headers);
    
    puts("== body ==");
    puts(resp.body);
    
}
