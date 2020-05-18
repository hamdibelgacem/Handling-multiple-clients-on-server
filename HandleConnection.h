#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <list>
#include <functional>

#include "Socket.h"
#include "pub-sub.cpp"

const int BUFSIZE {4096};

using namespace std;
using action_arguments = std::vector<std::string>;

class HandleConnection {
public:
	std::list<Subscriber> clients;
	fd_set readfds;
	Socket *masterSocket;
	std::vector<Publisher> rooms;

	HandleConnection(Socket *master);
	//~HandleConnection();
	int selectClient();
	void addClient();
	void disconnect();
	void receiveMessage();
	void splitCommands(char *buffer, string &command, std::vector<string> &params);
	void handleCommand(const string &command, const std::vector<string> &params, Subscriber &client);
	void addRoom(const string &name);
	bool isRoomCreated(const string &name);
	void subscribe(const std::vector<string> &params, Subscriber &client);
	void unsubscribe();
	void sine();
	void silence();
	void echo(const std::vector<string> &params);
};
