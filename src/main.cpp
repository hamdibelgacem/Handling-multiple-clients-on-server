#include <iostream>
#include <string>

#include "Socket.h"
#include "HandleConnection.h"

using namespace std;

int main(int argc, char **argv) {
    string ip = "localhost";
    
    if(argc < 2)
    {
		cout << "No port provided" << endl;
	}
    // portNum is for storing port number on which the accepts connections
    string port = argv[2];
    
    // Create the server socket 
    Socket *masterSocket = new Socket(AF_INET,SOCK_STREAM,0); //AF_INET (Internet mode) SOCK_STREAM (TCP mode) 0 (Protocol any)
    int optVal = 1;
    
    masterSocket->socket_set_opt(SOL_SOCKET, SO_REUSEADDR, &optVal);
    
    //Bind socket on ip:port
    masterSocket->bind(ip, port);

    //Start listening for incoming connections (maximum of 10 Connections in Queue).
    masterSocket->listen(10); 
	
	// Handling the connections between server and clinets. 
	HandleConnection connection(masterSocket);
   
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
