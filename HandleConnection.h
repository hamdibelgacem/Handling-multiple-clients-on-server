#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <list>
#include <functional>

#include "Socket.h"
#include "pub-sub.h"
#include "split.h"

// Size of buffer.
const int BUFSIZE {4096};

using namespace std;

typedef std::map<int, long> TokenMap;

class HandleConnection {
	std::list<Subscriber> clients;		// List of clients each clients.
	fd_set readfds;						// List of ready files descriptors.
	Socket *masterSocket;				// Socket master it's the server socket.
	std::vector<Publisher> rooms;		// List of rooms.
	std::map<string, TokenMap> tokens; 	// Store Subscribers tokens.

public:
	HandleConnection(Socket *master);
	//~HandleConnection();
	int selectClient();
	void addClient();
	void disconnect();
	void receiveMessage();
	void handleCommand(const string &command, const std::vector<string> &params, Subscriber &client);
	void addRoom(const string &name);
	bool isRoomCreated(const string &name);
	void subscribe(const std::vector<string> &params, Subscriber &client);
	void unsubscribe(const std::vector<string> &params, Subscriber &client);
	void sine();
	void silence();
	void echo(const std::vector<string> &params);
};
