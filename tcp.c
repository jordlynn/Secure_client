// tcp.c written by Jordan Lynn
/* TODO:
		- TCP connection
		- Test that IntPow() works correctly
		- Take over world
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

/* 
	To practice security across networks we'll use a
	basic key exchange, using a prime number as the
	modulus ( 17 ) and a primitive root of 17 which
	is 3. We do this because as we count up 3^x where
	x is 0 - 17 you will get an evenly likely answer for
	all the numbers 0 - 16. So basically no number is 
	more likely to occur than any other.
*/
#define BUFFERSIZE 256

// Prototypes
void Error( const char *msg );
int Calculate( int key );
int IntPow( int x, uint pow );


// Globals
int MOD = 17;
int GENERATOR = 3;

int main(int argc, char *argv []){
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	srand(time(NULL)); // Set seed for number generator.
	char buffer[BUFFERSIZE]; // Buffer to be sent across network.
	int privateKey = rand() % MOD; // Key to use in key exchange.
	int publicKey = Calculate( privateKey );
	printf("KEY1: %d\n", privateKey);
	printf("KEY: %d\n", publicKey);
	
	
return 0;
}

// IntPow just performs integer exponentiation.
int IntPow( int x, int pow ){
	int ret = 0;

	while( pow != 0 ){
		
		x = x * x;
		pow--;
	}
	ret = x;
	return ret;
}

/*  Calculate will take a private key, and exponetiate it to the generator
	after that then will perfor a modulus of "MOD" and return an interger
	that is then shared with the other client. 
*/ 
int Calculate( int key ){
	int publicAnswer; // Number to be return and ultimatly sent to other client.

	publicAnswer = IntPow( GENERATOR, key ) % MOD;
	return publicAnswer;
}


// error will print whatever error message the terminal receives
void Error( const char *msg ){
	perror( msg );
	exit(1);
}
