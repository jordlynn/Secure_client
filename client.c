
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

void InitializeClient( int argc, char *argv[], unsigned long key){
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];
   
	if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

   /* Obtain address(es) matching host/port */

   memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

   s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

   /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */

   for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

       if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

       close(sfd);
    }

   if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

   freeaddrinfo(result);           /* No longer needed */

   /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */
	// sending MOD, GENERATOR and key. 
	len = sizeof( MOD );	
	char temp[BUF_SIZE];
	sprintf( temp, "%lu", MOD );
	if( write(sfd, temp, len ) != len ) printf("Error sending MOD!\n"); // Send MOD

	len = sizeof( GENERATOR ); // Find the size of the buffer needed for the generator.
	sprintf( temp, "%lu", GENERATOR );  // Convert unsigned long into buffer
	if( write(sfd, temp, len ) != len ){ // Send away key.
		printf("Error sending key!\n");
	}

	len = sizeof( key ); // finally send key
	sprintf( temp, "%lu", key );
	if( write(sfd, temp, len ) != len ){ // Send away key.
		printf("Error sending key!\n");
	}

	// Send other message(s) that are in command line.
	for (j = 3; j < argc; j++) {
		len = strlen(argv[j]) + 1; // +1 for terminating null byte 

		if (len + 1 > BUF_SIZE) { // Catch issue where user's message is too long
			fprintf(stderr, "Ignoring long message in argument %d\n", j);
			continue;
		}

		/*	Some future error handling could have this if statement attempt to 
			re-intialize the socket if the write() fails based on the struct
			that contains the client's info.
		*/
		if (write(sfd, argv[j], len) != len) { // if write() returns 0 something is wrong.
			fprintf(stderr, "partial/failed write\n");
			exit(EXIT_FAILURE); // Instead of exit maybe re-create socket??
		}

		nread = read(sfd, buf, BUF_SIZE);
		if (nread == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		if( DEBUG ){	// Additional debugging info.	
			printf("\nBuffer has: %s\n", buf);
			printf("Received %ld bytes: %s\n", (long) nread, buf);
		}
    		// Now convert the key from the buffer into an unsgined long	
		key = strtoul( buf, NULL, 0); 	
		//if( KeyVerify( peerKey, key ) ) printf("Keys didn't match! D:\n");// Compare the two keys.	
	}

}

