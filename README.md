
# UDP File Transfer Projects

## Brief Overview

This repository contains two related projects developed for a Data Communications course.  
Both projects transfer files using UDP, but each demonstrates a different level of protocol complexity:

- Simple UDP: A minimal file-transfer system using a fixed initial port followed by dynamic port negotiation.
- Reliable UDP: A more advanced implementation that adds reliability mechanisms such as sequence numbers, parity checking, acknowledgments, and retransmission logic.

---

# Project 1: Simple UDP

## Description of the Project

This project implements a basic UDP file upload system.  
The client and server begin communication on a known port.  
The client sends an initial packet requesting a transfer and the server responds by sending back a new port number.  
Both the client and server then switch to this new port.  
The client sends the file to the server in fixed-size chunks.  
For each chunk received, the server prints the contents and sends back an uppercase version of the chunk.  
This provides simple confirmation to the client that the server received data.

## Data Flow

1. Client sends a handshake packet to the server on the initial port.  
2. Server replies with a dynamically chosen new port number.  
3. Client switches to the new port.  
4. Server binds and listens on the new port.  
5. Client sends file data in chunks to the new port.  
6. Server receives each chunk and sends back the uppercase version of the same chunk.  
7. When the file is fully sent, the client exits.  
8. Server terminates after processing the transfer.

## How to Use

1. Place client.cpp, server.cpp, and the Makefile in the same directory.

2. Compile using the make file
	``make``
3. Create a text file that you want to transfer.

4. Open two terminals and navigate to the directory.

5. Start the server:
	``./server <port_number>``

6. Start the client:
	`./client <ip_address> <port_number> <text_file>`

Example:

Terminal 1:
``./server 9001``

Terminal 2:
``./client localhost 9001 file.txt``

The server prints the random port number and the client prints the responses from the server.

# Project 2: Reliable UDP

## Description of the Project

Reliable UDP builds a custom reliability layer on top of UDP using an alternating bit protocol.  
It adds structured packet formats, parity-based error detection, ordered delivery guarantees, positive acknowledgments, retransmission behavior, and explicit end-of-transfer signaling.  
The system communicates through an external network emulator to test under simulated loss and corruption.

## Data Flow

(FixMe)

## How to Use

(FixMe)

---

# Extra Notes

- These projects manually implement transport behaviors over UDP to illustrate protocol design concepts.
- Project 1 demonstrates a minimal working protocol.
- Project 2 demonstrates reliability techniques without using TCP.

# Known Issues

- Simple UDP does not guarantee reliability.
- Reliable UDP uses fixed sleeps which slow transfers.
- Project 1 does not validate handshake contents.
- Project 2 does not include features such as sliding windows or congestion control.