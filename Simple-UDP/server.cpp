// Author: Granville Alpha NetID GBA35

#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <string.h> // using this to convert random port integer to string
#include <arpa/inet.h>
#include <unistd.h>
#include <random>
#include <ctype.h>
#include <fstream>
#define BUFFER_SIZE 1024
using namespace std;

int main(int argc, char *argv[]){
  
  struct sockaddr_in server;
  struct sockaddr_in client;
  int mysocket = 0;
  int checksocket = 0;
  socklen_t clen = sizeof(client);
  char payload[BUFFER_SIZE];
  char Handshake[8];
  char* n_portCHAR = (argv[1]);//Mutually agreed upon port
  int n_port = stoi(n_portCHAR);
  int r_port=-1;//random port variable  
  
  if ((checksocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    perror("1st Socket Creation");
  
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(n_port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  const char *expected = "ABCDEF";

  if (bind(checksocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    perror("Binding");
  if (recvfrom(checksocket, Handshake, BUFFER_SIZE, 0, (struct sockaddr *)&client, &clen)==-1){
    perror("Handshake Size");
    }
  if(strcmp(Handshake,expected)!=0){
    perror("Incorrect Handshake Content");
    //Handshake. The Payload should be equal to "ABCDEF"
    }
    
  
  random_device rd; // obtain a random number from hardware
  mt19937 gen(rd()); // seed the generator
  uniform_int_distribution<> distr(1024, 65535); // define the range
  //code used from 
  //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range

  r_port = distr(gen);//Dynamic port generation
  
  if(r_port==-1){perror("Rport Generation");}

  cout<<"The random port the server selected is: " << r_port << endl;

  sendto(checksocket, &r_port, 8, 0, (struct sockaddr *)&client, clen);
  //send random port #
  //now close the socket
  close(checksocket);

  if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1){
    perror("2nd Socket Creation");}
   //makes a new socket
  server.sin_port = htons(r_port);//sets to random port #
  
  if (bind(mysocket, (struct sockaddr *)&server, sizeof(server)) == -1){
    perror("2nd Binding");} //binds the new socket to the client
  
  ofstream newfile("upload.txt");//creates file to write to

  //Start of file transfer
  while(true){
    sleep(1);
    int payload_length=recvfrom(mysocket, payload, BUFFER_SIZE, 0, (struct sockaddr *)&client, &clen);
    //gets info
    if(payload_length<=0){
      break;
       }
    payload[payload_length]='\0';
    if (strcmp(payload, "END") == 0) {
      //checks to see end of file
        break;
    }
    newfile << payload;
    //writes to file
    for(int i=0; i<payload_length; i++){
      payload[i]=toupper(payload[i]);
      //Send uppercase Payload back to Client
      //Used for debugging
      //NOT proper error checking / retransmission
    }
    sendto(mysocket, payload, payload_length, 0, (struct sockaddr *)&client, clen);
    if(payload_length<4){
      break;
      }
    }
  newfile.close();   
  close(mysocket);
  return 0;
}
