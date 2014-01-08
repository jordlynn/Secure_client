/* File for intiating TCP connection with other client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <sys/wait.h>
#define BUF_SIZE 500

// Prototypes
void InitializeServer( int argc, char *argv[], unsigned long key );
void InitializeClient( int argc, char *argv[], unsigned long key );
void error( const char *msg );

// Both server and client need this function so I'll put it here.
void error( const char *msg ){
	perror(msg);
	exit(1);
}

// Structs
struct sockaddr_in serv_addr, cli_addr;
