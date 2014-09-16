/* echoClient.c - connects to a server, send a message, waits for a */
/*                response, and prints the response on stdout.      */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>

const char IP_ADDRESS[] = "127.0.0.1";
const int PORT_NUM = 2200;
const int DATA_SIZE = 256;
const char MSG[] = "A test message from the client\n";

int main()
{
  int serverSocket = 0; /*holds socket to connect to a server*/
  struct sockaddr_in serverAddress; /*holds address and port of a server*/
  char data[DATA_SIZE]; /*used to store message returned from server*/
  
  /* initialize and setup server address and port */
  bzero( &serverAddress, sizeof( serverAddress ) );
  serverAddress.sin_family = AF_INET;
  inet_aton( IP_ADDRESS, &serverAddress.sin_addr.s_addr );
  serverAddress.sin_port = htons( PORT_NUM );
  
  /* initialize data string to all zeros */
  bzero( data, DATA_SIZE );
  
  /* create IPv4 TCP socket */
  if ( (serverSocket = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
  {
    perror( "Error creating socket" );
    exit( 1 );
  }
  
  /* connect to the server */
  if ( connect( serverSocket, (struct sockaddr *)&serverAddress, sizeof( serverAddress ) ) < 0 )
  {
    perror( "Error connecting to server" );
    close( serverSocket );
    exit( 1 );
  }
  
  /* print the message to sdtout and send to the server */
  printf( "%s", MSG );
  if ( write( serverSocket, MSG, strlen( MSG )+1 ) < 0 )
  {
    perror( "Error writing" );
    close( serverSocket );
    exit( 1 );
  }
  
  /* wait for and read response from server then print to stdout */
  if ( read( serverSocket, data, DATA_SIZE ) < 0 )
  {
  perror( "Error reading" );
  close( serverSocket );
  exit( 1 );
  }
  else
  {
    printf("%s", data );
  }

  close( serverSocket );
  
  return 0;
}
