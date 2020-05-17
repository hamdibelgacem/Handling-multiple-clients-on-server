#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <functional>

#include "Socket.h"

const int BUFSIZE {4096};

using namespace std;

class HandleConnection {
	std::vector<int> client_socket;
	fd_set readfds;
	Socket *masterSocket;
public:
	HandleConnection(Socket *master);
	//~HandleConnection();
	int select_client();
	void add_client();
	void receiveMessage();
	void split_commands(char *buffer);
};
