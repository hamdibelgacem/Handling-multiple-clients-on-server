#include <iostream>
#include <string>

#include "Socket.h"

using namespace std;

int main() {
    string ip = "localhost";
    string port = "8888";
    Socket *masterSocket = new Socket(AF_INET,SOCK_STREAM,0); //AF_INET (Internet mode) SOCK_STREAM (TCP mode) 0 (Protocol any)
    int optVal = 1;
    
    masterSocket->socket_set_opt(SOL_SOCKET, SO_REUSEADDR, &optVal);
    masterSocket->bind(ip, port); //Bind socket on localhost:8888

    masterSocket->listen(10); //Start listening for incoming connections (maximum of 10 Connections in Queue)
	
	fd_set readfds;
	struct timeval tv;
	int secondes = 10; // set timeout 10 sec
	tv.tv_sec = secondes;
	tv.tv_usec = 0;
	std::vector<int> client_socket(100);
	char buffer[4096];  //data buffer 
	
	int max_sd = masterSocket->sock;
    
    while (true) {
		// initiailize my current set.
		FD_ZERO(&readfds);
		//add master socket to set
		FD_SET(masterSocket->sock, &readfds);
		
		for(int i = 0; i < client_socket.size(); ++i) {
            //socket descriptor  
            int sd = client_socket[i]; 
            
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd; 			
		}
		
		//wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely 
        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, &tv);
		
		if (activity < 0)
        {   
            cerr << "select error" << endl;
        }  
        
        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(masterSocket->sock, &readfds))   
        {   
            Socket *newSocket = masterSocket->accept();
                 
            //add new socket to array of sockets  
            for (int i = 0; i < client_socket.size(); i++)
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = newSocket->sock;   
                    cout << "Adding to list of sockets as " << i << endl;  
                    break;   
                }   
            }   
        }
        
        
        //else its some IO operation on some other socket 
        for (int i = 0; i < client_socket.size(); ++i)   
        {   
            int sd = client_socket[i];
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message
                memset(buffer, 0, 4096);
                int bytesIn = recv(sd, buffer, 4096, 0);
                if(bytesIn <= 0) {
                    //Somebody disconnected , get his details and print  
                    /*getpeername(sd , (sockaddr*)&address, (socklen_t*)&addrlen);
                    cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) 
                         << ", port " << ntohs(address.sin_port) << endl;
                    */     
                    //Close the socket and mark as 0 in list for reuse  
                    ::close(sd);   
                    client_socket[i] = 0;
                }
                     
                //Echo back the message that came in  
                else 
                {   
                    // Send message to other clients,
                    for(int i = 0; i < client_socket.size(); ++i) {
                        int outSock = client_socket[i];
                        if(outSock != masterSocket->sock && outSock != sd) {
                            ostringstream ss;
                            ss << "SOCKET #" << sd << ": " << buffer << "\r";
                            string strOut = ss.str();

                            send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                        }
                    } 
                }   
            }   
        }        
        
    }

    masterSocket->socket_shutdown(2);
    masterSocket->close();    
    return 0;
}
