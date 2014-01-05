#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <netdb.h>
#include <sys/wait.h>

// Defines


// Prototypes
void error( const char *msg );
void InitializeServer(  int argc, char **argv[] );
void InitializeClient(  int argc, char **argv[] );

// Structs
struct sockaddr_in serv_addr, cli_addr;

