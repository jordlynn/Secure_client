#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

// Defines
#define BUFFERSIZE 256 // size of buffer for networking.


// Prototypes
void error( const char *msg );
void InitializeConnection( int portno );

// Structs
struct sockaddr_in serv_addr, cli_addr;

