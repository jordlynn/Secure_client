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
void InitializeConnection( int portno ){
	socklen_t clilen;
	int sockfd, newsockfd;
	char buffer[BUFFERSIZE];
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
    serv_addr.sin_port = htons(portno);	

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

// Simple error function, call this for possible failures.
void error( const char *msg ){
	perror(msg);
	exit(1);
}
