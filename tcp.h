#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>

// Defines
#define BUFFERSIZE 256 // size of buffer for networking.


// Prototypes
void error( const char *msg );
void InitializeServer( char *portno );
void InitializeClient( char *portno );

// Structs
struct sockaddr_in serv_addr, cli_addr;

// Globals
int BUFFSIZE = 256;
