#include <iostream>
#include <string>

#include "Socket.h"
#include "HandleConnection.h"
#include "pub-sub.cpp"

using namespace std;

int main(int argc, char **argv) {
    string ip = "localhost";
    string port = "8888";
    Socket *masterSocket = new Socket(AF_INET,SOCK_STREAM,0); //AF_INET (Internet mode) SOCK_STREAM (TCP mode) 0 (Protocol any)
    int optVal = 1;
    
    masterSocket->socket_set_opt(SOL_SOCKET, SO_REUSEADDR, &optVal);
    masterSocket->bind(ip, port); //Bind socket on localhost:8888

    masterSocket->listen(10); //Start listening for incoming connections (maximum of 10 Connections in Queue)
	
	HandleConnection connection(masterSocket);
   
    while (true) { 
		connection.selectClient();
		connection.addClient();
		connection.receiveMessage();
    }
    
	/*
	Publisher room1("room1");
	Publisher room2("room2");
	Subscriber sub_1(1);
	Subscriber sub_2(2);
	
	long token_1 = Subscribe(room1, sub_1);
	long token_2 = Subscribe(room1, sub_2);

	room1.Publish("123");
	Unsubscribe(room1, token_1);
	room1.Publish("1234");
	room2.Publish("321");
	*/
    masterSocket->socket_shutdown(2);
    masterSocket->close();    
    return 0;
}
