/* echoServer.c - wait for a connection, read a message from the client, */
/*                print message to stdout and send back to the client.   */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>

const int PORT_NUM = 2200;
const int DATA_SIZE = 256;

int main()
{
  int serverSocket = 0, clientSocket = 0; /*holds socket to connect to a server and client*/
  int clientAddressSize = 0; /*place to store size of clientAddress structure*/
  struct sockaddr_in serverAddress, clientAddress; /*holds server and client address and port*/
  char dataIn[DATA_SIZE], dataOut[DATA_SIZE]; /*hold message from server and to client*/
  
  /* initialize and setup server address and port */
  bzero( &serverAddress, sizeof( serverAddress ) );
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl( INADDR_ANY );
  serverAddress.sin_port = htons( PORT_NUM );
  
  /* create IPv4 TCP socket */
  if ( (serverSocket = socket( AF_INET, SOCK_STREAM, 0 )) < 0 )
  {
    perror( "Error creating socket" );
    exit( 1 );
  }
  
  /* bind socket to address and port */
  if ( bind( serverSocket, (struct sockaddr *)&serverAddress, sizeof( serverAddress ) ) < 0 )
  {
    perror( "Error binding to socket" );
    close( serverSocket );
    exit( 1 );
  }
  
  /* listen for conection on socket.  Hold up to 5 connections in que */
  if ( listen( serverSocket, 5 ) < 0 )
  {
    perror( "Error listening to socket" );
    close( serverSocket );
    exit( 1 );
  }
  
  /* ininitialize client structure and size variable */
  bzero( &clientAddress, sizeof( clientAddress ) );
  clientAddressSize = sizeof( clientAddress );
  
  /* accept conection from client */
  if ( (clientSocket = accept( serverSocket, (struct sockaddr *)&clientAddress, &clientAddressSize )) < 0 )
  {
    perror( "Error connecting to client" );
    close( serverSocket );
    exit( 1 );
  }
  
  /* initialize data strings to all zeros */
  bzero( dataIn, DATA_SIZE );
  bzero( dataOut, DATA_SIZE );
  strncat( dataOut, "From Server: ", DATA_SIZE );
  
  /* read in message from client */
  if ( read( clientSocket, dataIn, DATA_SIZE ) < 0 )
  {
  perror( "Error reading" );
  close( clientSocket );
  close( serverSocket );
  exit( 1 );
  }
  else
  {
    /* print message from client to stdout and send back to client */
    printf( "From client: %s", dataIn );
    strncat( dataOut, dataIn, DATA_SIZE );
    if ( write( clientSocket, dataOut, strlen( dataOut )+1 ) < 0 )
    {
      perror( "Error writing" );
      close( clientSocket );
      close( serverSocket );
      exit( 1 );
    }
  }

  close( clientSocket );
  close( serverSocket );
  
  return 0;
}
