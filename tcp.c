/* File for intiating TCP connection with other client */
#include "tcp.h"

/* Creates the server side of the client for the two way communcation and waits for
	a connection. Either one of the people in the chat needs to send the key to
	the other user, when this happens the program grabs the address of the other
	user and will save that. So there isn't a need to enter the actual address
	on both sides, only one side needs to and that happens when the key is sent.
	
	A lot of this function uses sys/socket.h so if you want more info on how
	these functions and calls are working google the header file.
*/
void InitializeServer( char *portno ){
	socklen_t clilen;
	int sockfd, newsockfd;
	char buffer[BUFFSIZE];
	int n;
	// TODO: Do error handling to check portno ect..
	
	printf("Starting socket...\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0); // Assign socket
	if ( sockfd < 0){ // socket() will return < 0 if an error occured.
		printf("ERROR: couldn't bind socket!\n");
		return;
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));	
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //serv_addr.sin_port = htons(portno);	

	if ( bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	
	listen(sockfd,5); // Server is now up and listening!
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
	if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
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
    char buf[BUFFSIZE];
	char *address;	

	memset(&hints, 0, sizeof(struct addrinfo)); // Alocate memory for addrinfo
    hints.ai_family = AF_UNSPEC;    // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;// Use a TCP connection, switch to UDP for funsies later?
    hints.ai_flags = AI_PASSIVE;	// For wildcard IP address 
    hints.ai_protocol = 0;			// Any protocol 
	
	printf("Please enter the server's addres: ");
	scanf("%p", &address);	

	s = getaddrinfo(address, portno, &hints, &result); // Here's the magic!
    if (s != 0) { // If the address failed
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
	
	/* getaddrinfo() returns a list of address structures. So we need to try
		every address until we connect, if socket or connect fail we close
		the socket and move onto the next address.
		This is exactly like traversing any other type of LL.
	*/		
	for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
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
	
	freeaddrinfo(result); // No longer needed since we have our socket.

	/* Send remaining command-line arguments as separate
       datagrams, and read responses from server 
	*/

	for (j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;
                // +1 for terminating null byte

       if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

       if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

       nread = read(sfd, buf, BUF_SIZE);
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
