#include <iostream>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <netdb.h> 
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <string>
#define BUFFER_SIZE 1024
using namespace std;

int main(int argc, char *argv[]){
  struct hostent *s;
  struct sockaddr_in server;
  socklen_t slen = sizeof(server);
  s = gethostbyname(argv[1]);   //gets local host name 
  char* n_portCHAR = (argv[2]); //mutually agreed upon port
  int n_port = stoi(n_portCHAR);//changes char to an int
  int mysocket = 0;
  int checksocket=0; 
  int r_port=-1;//rport number to receive
  
  char Handshake[8]="ABCDEF"; 

  if ((checksocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    perror("1st Socket");
    
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(n_port);
  bcopy((char *)s->h_addr, 
	(char *)&server.sin_addr.s_addr,
	s->h_length);
  
  if (sendto(checksocket, Handshake, 8, 0, (struct sockaddr *)&server, slen)==-1)
    perror("Sending Handshake");
      //sending handshake to server
  sleep(2);

  recvfrom(checksocket, &r_port, 8, 0, (struct sockaddr *)&server, &slen); 
  //Recives Dynamic Port number
  
  close(checksocket); 
  //close old socket

   if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    perror("2nd Socket Creation");
    //creates a new socket that will be used for file transfer
    
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(r_port);
  bcopy((char *)s->h_addr, (char *)&server.sin_addr.s_addr, s->h_length);
  //fills in all the information into the socket

  size_t numbytes = 4; 
  char payload[numbytes];
  ifstream userfile(argv[3]);
  //gets file name
  //file transfer functionality
  if(userfile.is_open()){
    while(userfile.read(payload,numbytes)|| userfile.gcount()>0){  
      int bytesRead = userfile.gcount();
      sendto(mysocket, payload, bytesRead, 0, (struct sockaddr *)&server, slen);
      //sends 4 bytes read from file
      sleep(1);
      recvfrom(mysocket, payload, BUFFER_SIZE, 0, (struct sockaddr *)&server, &slen);
      //recieves back the content that was sent to server
      cout << payload << endl;
      //prints the recieved text from the server
      //Used for debugging not a proper Resend protocol
      
      if(bytesRead<4){
        sendto(mysocket, "END", 3, 0, (struct sockaddr *)&server, slen);
        //sends end statement
        break;
     }  
    }
    sendto(mysocket, "END", 3, 0, (struct sockaddr *)&server, slen);
    userfile.close();
   }
  close(mysocket);
  return 0;
}