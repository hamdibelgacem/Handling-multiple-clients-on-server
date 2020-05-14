#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros  

const int PORT = 54000;

using namespace std;

int main() {

    sockaddr_in address;

    const int max_clients = 100;
    int client_socket[max_clients];
    int new_socket, sd, max_sd, valread, addrlen;
    
    //set of socket descriptors  
    fd_set readfds; 
    int opt = 1;

    char buffer[4096];  //data buffer of 

    //type of socket created  
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;   
    address.sin_port = htons( PORT ); 

    // Create a Socket.
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);
    if( -1 == master_socket) {
        cerr << "Can't create a socket!" << endl;
        return -1;
    }

    //set master socket to allow multiple connections ,  
    //this is just a good habit, it will work without this
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {   
        cerr << "setsockopt failed!" << endl;  
        return -2;
    }  

    if( -1 == bind(master_socket, (sockaddr*)&address, sizeof(address)) ) {
        cerr << "Can't bind to IP/Port" << endl;
        return -3;
    }

    cout << "Listener on port " << PORT << endl;

    // Mark the socket for listening in
    if( -1 == listen(master_socket, SOMAXCONN)) {
        cerr << "Can't listen!" << endl;
        return -4;
    }

    cout << "Waiting for connections ..." << endl;

    //initialise all client_socket[] to 0 so not checked  
    for (int i = 0; i < max_clients; i++){   
        client_socket[i] = 0;   
    }   

    while (true)
    {
        //clear the socket set          
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set  
        for (int i = 0 ; i < max_clients ; ++i)   
        {   
            //socket descriptor  
            sd = client_socket[i];   
            
            //if valid socket descriptor then add to read list  
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            //highest file descriptor number, need it for the select function  
            if(sd > max_sd)   
                max_sd = sd;   
        } 

        //wait for an activity on one of the sockets , timeout is NULL ,  
        //so wait indefinitely 
        int activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);
        
        if ((activity < 0) && (errno!=EINTR))   
        {   
            cerr << "select error" << endl;
        }  

        //If something happened on the master socket ,  
        //then its an incoming connection  
        if (FD_ISSET(master_socket, &readfds))   
        {   
            new_socket = accept(master_socket, (sockaddr*)&address, (socklen_t*)&addrlen);
            if ( new_socket < 0 )   
            {   
                cerr << "Problem with client connecting!" << endl;  
                exit(EXIT_FAILURE);  
            }
            
            //inform user of socket number - used in send and receive commands  
            cout << "New connection , socket fd is " << new_socket
                << ", ip is : " << inet_ntoa(address.sin_addr)
                << ", port : " << ntohs (address.sin_port) << endl;
           
            //send new connection greeting message  
            string welcomeMsg = "Welcome to chat room!\r\n"; 
            send(new_socket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
            cout << "Welcome message sent successfully" << endl;
                 
            //add new socket to array of sockets  
            for (int i = 0; i < max_clients; i++)   
            {   
                //if position is empty  
                if( client_socket[i] == 0 )   
                {   
                    client_socket[i] = new_socket;   
                    cout << "Adding to list of sockets as " << i << endl;  
                    break;   
                }   
            }   
        }   
             
        //else its some IO operation on some other socket 
        for (int i = 0; i < max_clients; ++i)   
        {   
            sd = client_socket[i];
                 
            if (FD_ISSET( sd , &readfds))   
            {   
                //Check if it was for closing , and also read the  
                //incoming message
                memset(buffer, 0, 4096);
                int bytesIn = recv(sd, buffer, 4096, 0);
                // if ((valread = read( sd , buffer, 4096)) == 0)   
                // {   
                //     //Somebody disconnected , get his details and print  
                //     getpeername(sd , (sockaddr*)&address, (socklen_t*)&addrlen);
                //     cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) 
                //          << ", port " << ntohs(address.sin_port) << endl;
                         
                //     //Close the socket and mark as 0 in list for reuse  
                //     close( sd );   
                //     client_socket[i] = 0;   
                // }   

                if(bytesIn <= 0) {
                    //Somebody disconnected , get his details and print  
                    getpeername(sd , (sockaddr*)&address, (socklen_t*)&addrlen);
                    cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) 
                         << ", port " << ntohs(address.sin_port) << endl;
                         
                    //Close the socket and mark as 0 in list for reuse  
                    close( sd );   
                    client_socket[i] = 0;

                }
                     
                //Echo back the message that came in  
                else 
                {   
                    //set the string terminating NULL byte on the end  
                    //of the data read  
                    
                    buffer[valread] = '\0'; 
                    send(sd , buffer , strlen(buffer) , 0 );   
                }   
            }   
        } 
    }
    
    return 0;
}