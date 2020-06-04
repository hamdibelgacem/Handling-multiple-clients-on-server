#include <iostream>
#include <string>
#include <memory>

#include "Socket.h"
#include "HandleConnection.h"

using namespace std;

int main(int argc, char **argv) {
    
    if(argc < 2) // We expect 2 arguments: the program name an port number.
    {
		std::cerr << "Usage : " << argv[0] << " PORT" <<  endl;
	}
	
	string ip = "localhost";
	
    // portNum is for storing port number on which the accepts connections
    string port = argv[1];
    
    // Create the server socket 
    std::unique_ptr<Socket> masterSocket{ std::make_unique<Socket>(AF_INET,SOCK_STREAM,0) }; //AF_INET (Internet mode) SOCK_STREAM (TCP mode) 0 (Protocol any)
    int optVal = 1;
    
    masterSocket->socket_set_opt(SOL_SOCKET, SO_REUSEADDR, &optVal);
    
    //Bind socket on ip:port
    masterSocket->bind(ip, port);

    //Start listening for incoming connections (maximum of 10 Connections in Queue).
    masterSocket->listen(10); 
	
	// Handling the connections between server and clinets. 
	HandleConnection connection(std::move(masterSocket));
   
    while (true) { 
		
		// Select waiting until one or more of the file descriptors become "ready"
		connection.selectClient();
		
		// If file descriptor ready i.e will have to added in list of readfds.
		// And added this client into clients list and identify by Socket ID.
		connection.addClient();
		
		// Handling of messages received.
		connection.receiveMessage();
    }
    
    // Suhtdown master socket and close masterSocket.
    masterSocket->socket_shutdown(2);
    masterSocket->close();
    
    return 0;
}
