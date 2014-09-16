/* webServer.c - demonstrates an extremely simple web server with a single static web page */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>

const int PORT_NUM = 8088;
const int DATA_SIZE = 2048;
const char WEB_PAGE[] = "<!DOCTYPE html>\
<html lang='en'>\
  <head>\
    <meta charset='utf-8' />\
    <title>Hello World Web Server</title>\
  </head>\
  <body>\
    <h1>Hello World Web Server</h1>\
  </body>\
</html>";

int main()
{
  int serverSocket = 0, clientSocket = 0;
  int clientAddressSize = 0;
  struct sockaddr_in serverAddress, clientAddress;
  char dataIn[DATA_SIZE], dataOut[DATA_SIZE];
  char webPageLength[20];
  
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
  
  /* build response */
  sprintf( webPageLength, "%d", (int)strlen( WEB_PAGE ) );
  strncat( dataOut, "HTTP/1.0 200 OK\r\n", DATA_SIZE );
  strncat( dataOut, "Content-Type: text/html\r\n", DATA_SIZE );
  strncat( dataOut, "Content-Length: ", DATA_SIZE );
  strncat( dataOut, webPageLength, DATA_SIZE );
  strncat( dataOut, "\r\n", DATA_SIZE );
  strncat( dataOut, "\r\n", DATA_SIZE );
  strncat( dataOut, WEB_PAGE, DATA_SIZE );
  
  /* read in request from web client */
  if ( read( clientSocket, dataIn, DATA_SIZE ) < 0 )
  {
  perror( "Error reading" );
  close( clientSocket );
  close( serverSocket );
  exit( 1 );
  }
  else
  {
    /* print request from web client to stdout */
    printf( "From client: \n%s\n", dataIn );
    
    /* send web page to client */
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
