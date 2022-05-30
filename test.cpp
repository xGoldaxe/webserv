// #include <iostream>
// #include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <netinet/tcp.h>   /* TCP_KEEPIDLE, etc. */
#include <arpa/inet.h>


#define D_PORT	3000

int main( void ) {

	int port = D_PORT;
 	struct sockaddr_in client;         
    int sock;  

    client.sin_family = AF_INET;  
	client.sin_addr.s_addr = htonl(INADDR_LOOPBACK);      

	client.sin_port = htons( port );  
	sock = (int) socket(AF_INET, SOCK_STREAM, 0);
	int result = connect(sock, (struct sockaddr *) &client,sizeof(client));
	
	while ( result != 0 )
	{
		++port;
		client.sin_port = htons( port );  
		sock = (int) socket(AF_INET, SOCK_STREAM, 0);
		int result = connect(sock, (struct sockaddr *) &client,sizeof(client));
	}

	// std::cout << "ew";
	// std::cout << to_string( port );

	return (0);
}