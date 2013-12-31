// tcp.c written by Jordan Lynn
/* TODO:
		- TCP connection
		- Take over world
		- Spawn child process to setup TCP connection
 
	To practice security across networks we'll use a
	basic key exchange, using a prime number as the
	modulus ( 17 ) and a primitive root of 17 which
	is 3. We do this because as we count up 3^x where
	x is 0 - 17 you will get an evenly likely answer for
	all the numbers 0 - 16. So basically no number is 
	more likely to occur than any other.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tcp.c"


// Prototypes
unsigned long Calculate( unsigned long key );
unsigned long IntPow( unsigned long x, unsigned long pow, unsigned long mod );
void SetupConnection();

// Globals
unsigned long MOD = 17;
unsigned long GENERATOR = 3;
char *PORTNUM = "8080";

int main(int argc, char *argv []){
	srand(time(NULL)); // Set seed for number generator.
	unsigned long privateKey = rand() % MOD; // Key to use in key exchange.
	unsigned long publicKey = Calculate( privateKey );
	int temp = 0;
	
	// First we'll ask the user how this will get setup.
	printf("Hello! Would you like to be a server(1) or client(2)? ");
	scanf("%d", &temp);
	if( temp == 1 ) InitializeServer( PORTNUM ); // Start TCP connection.
	else if( temp == 2 ) InitializeClient( PORTNUM );
	else printf("Sorry I didn't understand your choice!\n");

return 0;
}

// IntPow just performs integer exponentiation.
unsigned long IntPow( unsigned long x, unsigned long pow, unsigned long mod ){
	unsigned long temp = x;
	
	while( pow > 1 ){	
		temp = (temp * x) % mod; // Multiply answer by the key value (x)
		pow--; // decrement counter
	}
	return temp; // Assign final answer
}

/*  Calculate will take a private key, and exponetiate it to the generator
	after that then will perfor a modulus of "MOD" and return an interger
	that is then shared with the other client. 
*/ 
unsigned long Calculate( unsigned long key ){
	unsigned long publicAnswer; // Number to be return and ultimatly sent to other client.
	
	publicAnswer = (IntPow( GENERATOR, key, MOD));
	// printf("GOT %lu\n", publicAnswer); // Debugging printf
	return publicAnswer;
}
