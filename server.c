
/* Creates the server side of the client for the two way communcation and waits for
	a connection. Either one of the people in the chat needs to send the key to
	the other user, when this happens the program grabs the address of the other
	user and will save that. So there isn't a need to enter the actual address
	on both sides, only one side needs to and that happens when the key is sent.
	
	A lot of this function uses sys/socket.h so if you want more info on how
	these functions and calls are working google the header file.

	If you're looking for a way to debug go to the main.h file and set the "DEBUG" value to 1. 
*/

void InitializeServer(int argc, char *argv[], unsigned long key){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char buf[BUF_SIZE];

	if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

   s = getaddrinfo(NULL, argv[1], &hints, &result);
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
	int count = 0; // Used to decided what we're recieving, first three packets shoud be info.
   while (peerKey == 99) {
        
	peer_addr_len = sizeof(struct sockaddr_storage);
        nread = recvfrom(sfd, buf, BUF_SIZE, 0,
                (struct sockaddr *) &peer_addr, &peer_addr_len);
	
	unsigned long peerInfo = strtoul( buf, NULL, 0); // Now convert the buffer into unsigned long for key.
	switch (count){
		case 0:
			MOD = peerInfo;
			count++;
			break;
		case 1:
			GENERATOR = peerInfo;
			count++;
			break;
		case 2:
			peerKey = peerInfo;
			count++;
			break;
		default:
			break;
	}
	
	if (nread == -1)
            continue;               /* Ignore failed request */

       char host[NI_MAXHOST], service[NI_MAXSERV];

       s = getnameinfo((struct sockaddr *) &peer_addr, // obtain info from peer.
                        peer_addr_len, host, NI_MAXHOST,
                        service, NI_MAXSERV, NI_NUMERICSERV);
       if (s == 0 || DEBUG) // Show peer's info if needed.
            printf("Received %ld bytes from %s:%s\n",
                    (long) nread, host, service);
        else
            fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
	
	// Store the server's key into the buffer and send it back.
	sprintf( buf, "%lu", key );
       if (sendto(sfd, buf, nread, 0,
                    (struct sockaddr *) &peer_addr,
                    peer_addr_len) != nread)
            fprintf(stderr, "Error sending response\n");
	
	}

	if( DEBUG ) printf("Got\n MOD: %ld\n GENERATOR: %ld\n Key: %ld\n", MOD, GENERATOR, peerKey);
}

