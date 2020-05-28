#include <iostream>
#include <string.h>
#include <iomanip>      // std::setw
#include "cmath"
#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

//Create a Socket for server communication
short SocketCreate(void)
{
    short Socket;
    cerr << "Create the socket" << endl;
    Socket = socket(AF_INET, SOCK_STREAM, 0);
    return Socket;
}

//try to connect with server
int SocketConnect(int Socket, int port)
{
    int iRetval=-1;
    int ServerPort = port;
    struct sockaddr_in remote= {0};
    remote.sin_addr.s_addr = inet_addr("127.0.0.1"); //Local Host
    remote.sin_family = AF_INET;
    remote.sin_port = htons(ServerPort);
    iRetval = connect(Socket,(struct sockaddr *)&remote,sizeof(struct sockaddr_in));
    return iRetval;
}

// Send the data to the server and set the timeout of 20 seconds
int SocketSend(int Socket,std::string Rqst,short lenRqst)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 10;  /* 10 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(Socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        cerr << "Time Out" << endl;
        return -1;
    }
    shortRetval = send(Socket, Rqst.c_str(), lenRqst, 0);
    return shortRetval;
}

//receive the data from the server
int SocketReceive(int Socket,char* Rsp,short RvcSize)
{
    int shortRetval = -1;
    struct timeval tv;
    tv.tv_sec = 10;  /* 10 Secs Timeout */
    tv.tv_usec = 0;
    if(setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)) < 0)
    {
        cerr << "Time Out" << endl;
        return -1;
    }
    shortRetval = recv(Socket, Rsp, RvcSize, 0);
    return shortRetval;
}

//main driver program
int main(int argc, char *argv[])
{
    int Socket, bytesReceived;
    int port = 8888;
    if(argv[2])
		port = atoi(argv[2]);
    // struct sockaddr_in server;
    string SendToServer;
    char serverReply[4096];

    //Create socket
    Socket = SocketCreate();
    if(Socket == -1)
    {
        cerr << "Could not create socket" << endl;
        return -1;
    }
    cout << "Socket is created" << endl;
    
    //Connect to remote server
    if (SocketConnect(Socket, port) < 0)
    {
        cerr << "connect failed." << endl;
        return -1;
    }
    cout << "Sucessfully conected with server" << endl;
    
    while(true)
    {
		getline(std::cin, SendToServer);
		
		//Send data to the server
		int sendResult = SocketSend(Socket, SendToServer, SendToServer.size() + 1);
		
		//Received the data from the server
		if(sendResult < 0)
		{
			cerr << "Send failed" << endl;
		}
		else
		{
			//cout << "received msg: ";
			memset(serverReply, 0, 4096);
			bytesReceived = SocketReceive(Socket, serverReply, 4096);
			
			if(bytesReceived == 0)
			{
				close(Socket);
			}
			else if(bytesReceived > 0)
			{	
				std::vector<uint16_t> audio_data;
				for(int i = 0; i < bytesReceived - 1; ++i)
				{
					if(i%2 == 0)
					{
						uint16_t v = (serverReply[i] << 8) | serverReply[i+1];
						cout << std::hex << setw(4) << setfill('0') << v;
					}
				}
			}
		}

	}

    
    close(Socket);
    shutdown(Socket,0);
    shutdown(Socket,1);
    shutdown(Socket,2);
    return 0;
}
