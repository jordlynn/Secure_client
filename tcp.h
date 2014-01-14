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
int KeyVerify( unsigned long peerKey, unsigned long key ); 
void PrintInfo(unsigned long privateKey);
unsigned long Pow( unsigned long x, unsigned long pow, unsigned long mod );
// Both server and client need these functions so I'll put them here.


void error( const char *msg ){
	perror(msg);
	exit(1);
}

int KeyVerify( unsigned long peerKey, unsigned long privateKey ){
	unsigned long temp;
	temp = Pow( peerKey, privateKey, MOD ); 
	printf("TEMP: %lu\n", temp);
	
	return 0;
}

unsigned long Pow( unsigned long x, unsigned long pow, unsigned long mod ){
	unsigned long temp = x;
	
	while( pow > 1 ){	
		temp = (temp * x) % mod; // Multiply answer by the key value (x)
		pow--; // decrement counter
	}
	return temp; // Assign final answer
}


void PrintInfo(unsigned long privateKey){
	if( DEBUG ){
		printf("Got\n MOD: %lu\n GENERATOR: %lu\n peerKey: %lu\n privateKey: %lu\n", 
		MOD, GENERATOR, peerKey, privateKey);
		
	}
}

// Structs
struct sockaddr_in serv_addr, cli_addr;
