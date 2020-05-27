#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <list>
#include <functional>
#include <memory>
#include <bitset>
#include "cmath"

#include "Socket.h"
#include "pub-sub.h"
#include "SilentAudioChunk.h"
#include "SineWaveAudioChunk.h"
#include "AudioChunk.h"
#include "split.h"

// Size of buffer.
const int BUFSIZE {4096};

using namespace std;

typedef std::map<int, long> TokenMap;

class HandleConnection {
	std::list<Subscriber> clients;			// List of clients each clients.
	fd_set readfds;							// List of ready files descriptors.
	std::unique_ptr<Socket> masterSocket; 	// Socket master it's the server socket.
	std::vector<Publisher> rooms;			// List of rooms.
	std::map<string, TokenMap> tokens; 		// Store Subscribers tokens.

public:
	HandleConnection(std::unique_ptr<Socket> masterSocket);
	//~HandleConnection();
	int selectClient();
	void addClient();
	void receiveMessage();
	void handleCommand(const string &command, const std::vector<string> &params, Subscriber &client);
	void addRoom(const string &name);
	bool isRoomCreated(const string &name);
	void subscribe(const std::vector<string> &params, Subscriber &client);
	void unsubscribe(const std::vector<string> &params, Subscriber &client);
	void sine(const std::vector<string> &params, const Subscriber &client);
	void silence(const std::vector<string> &params, const Subscriber &client);
	void echo(const std::vector<string> &params, const Subscriber &client);
	void disconnect();
};
