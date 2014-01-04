/* File for intiating TCP connection with other client */
#include "tcp.h"
#include "main.h"

/* Creates the server side of the client for the two way communcation and waits for
	a connection. Either one of the people in the chat needs to send the key to
	the other user, when this happens the program grabs the address of the other
	user and will save that. So there isn't a need to enter the actual address
	on both sides, only one side needs to and that happens when the key is sent.
	
	A lot of this function uses sys/socket.h so if you want more info on how
	these functions and calls are working google the header file.

	If you're looking for a way to debug go to the main.h file and set the "DEBUG" value to 1. 
*/
void InitializeServer( char *portno ){
	
	struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUFFSIZE];

   memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
	printf("starting server...\n");
   s = getaddrinfo(NULL, portno, &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

   /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */

   for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                rp->ai_protocol);
        if (sfd == -1)
            continue;

       if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

       close(sfd);
    }

   if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

   freeaddrinfo(result);           /* No longer needed */

   /* Read datagrams and echo them back to sender */

   for (;;) {
        peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUFFSIZE, 0,
                (struct sockaddr *) &peer_addr, &peer_addr_len);
        if (nread == -1)
            continue;               /* Ignore failed request */

       char host[NI_MAXHOST], service[NI_MAXSERV];

       s = getnameinfo((struct sockaddr *) &peer_addr,
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);
       if (s == 0)
            printf("Received %ld bytes from %s:%s\n",
                    (long) nread, host, service);
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));

       if (sendto(sfd, buf, nread, 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");
    }

}

/* Creates the client side of the two way communcation and will 
	intiate the communication. It will first make sure everyone
	has agreed on the same public key, then will exchange it's 
	public key(s) and start the encypted communcation.
			
BIG CHANGE BELOW!
	Originally the function would call "gethostbyname()" but as I read about
		it more and more it seems outdated, it will even check IPv4 addresses.
		So in the name of being current and up-to-date we'll use it's successor
		"getaddinfo" which is actually pretty cool.
*/	

void InitializeClient( char *portno ){
	   
	// Because of the switch to "getaddrinfo()" we need a struct to hold info on connection.
	struct addrinfo hints; // Holds specs of connection addept.
	struct addrinfo *result, *rp; // will return with point to connection!
	int sfd, s, j;
	size_t len;
	ssize_t nread;
	char buf[BUFFSIZE]; // Used to recieve messages.
	char testM [128];
	char *address = testM; // Holds address/IPv4/IPv6 of other client.	
	int childPID; // Used when forking.
	int *retStatus = NULL;

	memset(&hints, 0, sizeof(struct addrinfo)); // Alocate memory for addrinfo
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;// Use a TCP connection, switch to UDP for funsies later?
    hints.ai_flags = AI_PASSIVE;	// For wildcard IP address 
    hints.ai_protocol = 0;			// Any protocol 
	
	printf("Please enter the server's addres: "); // prompt for address, store.
	scanf("%s", address);
	
	if( DEBUG ) printf("Atempting to connect to: %s\n", address);	

	childPID = fork(); // Fork off and step into process child.
	
	// Now if we are the child continue else wait (parent)
	if( childPID ) childPID = wait(retStatus);
	
	/* Sorry about the curly brackets on the else statement, my screen isn't large enough
	   and I'm having problems getting things to match up but can't see the top and bottom
	   it works, just around the end of the connection stuff the brackets get freaky.
	*/	
	else{	
		// We should now be in the child...
		// The following sleep is used to help with debugging with gdb.
		if( DEBUG ) sleep(15);
	
		s = getaddrinfo(address, portno, &hints, &result); // Here's the magic!
		if (s != 0) { // If the address failed
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
			exit(-1);
    		}
		/* getaddrinfo() returns a list of address structures. So we need to try
			every address until we connect, if socket or connect fail we close
			the socket and move onto the next address.
			This is exactly like traversing any other type of LL.
		*/		
		for (rp = result; rp != NULL; rp = rp->ai_next) {
			sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
			if (sfd == -1)
			continue;

			if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;                  /* Success */

			close(sfd);
		}

	// If rp == NULL then we didn't find any addresses.
	 if (rp == NULL) {
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    } 
	}
	
	freeaddrinfo(result); // No longer needed since we have our socket.

	/* Send remaining command-line arguments as separate
       datagrams, and read responses from server 
	*/

	for (j = 3; j < BUFFSIZE ; j++){
        len = strlen(testM) + 1;
                // +1 for terminating null byte

       if (len + 1 > BUFFSIZE){
            fprintf(stderr,"Uh oh! Message exceeded buffer! %d\n", j);
            continue;
        }

       if (write(sfd, testM, len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

       nread = read(sfd, buf, BUFFSIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
		printf("Received %ld bytes: %s\n", (long) nread, buf);
    }
}
// Simple error function, call this for possible failures.
void error( const char *msg ){
	perror(msg);
	exit(1);
}
