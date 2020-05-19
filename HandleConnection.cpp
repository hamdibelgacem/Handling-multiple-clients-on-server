#include "HandleConnection.h"

HandleConnection::HandleConnection(Socket *master) : masterSocket(master) {}

int HandleConnection::selectClient() {
	struct timeval tv;
	int secondes = 10; // set timeout 10 sec
	tv.tv_sec = secondes;
	tv.tv_usec = 0;
	int max_sd = masterSocket->sock;
	FD_ZERO(&readfds); // initiailize my current set.
	FD_SET(masterSocket->sock, &readfds); //add master socket to set
	for(const auto &client : clients) {
		int sd = client.getSocket(); //socket descriptor
		
		//if valid socket descriptor then add to read list  
		if(sd > 0)   
			FD_SET( sd , &readfds);   
			 
		//highest file descriptor number, need it for the select function  
		if(sd > max_sd)   
			max_sd = sd; 			
	}
	
	//wait for an activity on one of the sockets , timeout is 10 sec
	int ret = select(max_sd + 1, &readfds, nullptr, nullptr, &tv);
	
	if (ret < 0)
	{   
		cerr << "select error" << endl;
	} 
	
	return ret; 	
}

void HandleConnection::disconnect() {
	auto it = clients.begin();
	while(it != clients.end()) {
		it = find_if(it, clients.end(), [=](Subscriber s) { return s.getSocket() == 0; } );
		// remove client
		// todo : unsubscribe this client from the subscriber rooms
		clients.erase(it);
		it++;
	}
}

void HandleConnection::addClient() {
	//If something happened on the master socket, then its an incoming connection  
	if (FD_ISSET(masterSocket->sock, &readfds))   
	{   
		Socket *newSocket = masterSocket->accept();
		// Adding client to Clients list.
		int socket = newSocket->sock;
		Subscriber subscriber(socket);
		clients.push_back(subscriber);
	}	
}

void HandleConnection::subscribe(const std::vector<string> &params, Subscriber &client) {
	for(const auto & param : params) {
		if(!isRoomCreated(param)) {
			cout << "------- room " << param << " is not created ------ " << param << endl;
			addRoom(param);
		} else {
			cout<<"------- Room "<< param << " is created -------- "<<param<<endl;
		}
		
		auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == param;} );
		if(it != rooms.end()) {
			long token = Subscribe(*it, client);
			std::map<string, long> pubTokens;
			pubTokens.insert(make_pair(it->Name(), token));
			for(map<string,long>::const_iterator eptr=pubTokens.begin();eptr != pubTokens.end(); eptr++)
				cout << "pubTokens : " << eptr->first << "  " << eptr->second << endl;
			tokens.insert(make_pair(client.getSocket(), pubTokens));
		}
		
	}
}

void HandleConnection::unsubscribe(const std::vector<string> &params, Subscriber &client) {
	for(const auto &param : params) {
		auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == param;} );
		auto postionClient = find_if(clients.begin(), clients.end(), [=](Subscriber s){ return s.getSocket() == client.getSocket();} );
		if(it != rooms.end())
			Unsubscribe(*it, std::distance(clients.begin(), postionClient)+1);
	}		
}

void HandleConnection::echo(const std::vector<string> &params) {
	auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == params[0];} );
	it->Publish("123");
}

void HandleConnection::handleCommand(const string &command, const std::vector<string> &params, Subscriber &client) {
	
	if("subscribe"  == command) {
		subscribe(params, client);
		cout << "\n===============tokens=============" << endl;		
		for(auto token : tokens)
		{
			for(auto t : token.second)
				std::cout << token.first << " " << t.first << " " << t.second << "\n";
		}	
	}
	else if("unsubscribe" == command) {
		unsubscribe(params, client);
	}
	else if ("sine" == command) {
		//sine();
	}
	else if("silence" == command) {
		//silence();
	}
	else if("echo" == command) {
		echo(params);
	}
	else {
		cout << "command not found !" << endl;
	}
}

void HandleConnection::receiveMessage() {
	//some IO operation on some other socket 
	char buffer[BUFSIZE];  //data buffer 
	
	for (auto &client : clients)   
	{   
		int sd = client.getSocket();
			 
		if (FD_ISSET( sd , &readfds))   
		{   
			//Check if it was for closing, and also read the incoming message.
			memset(buffer, 0, BUFSIZE);
			int bytesIn = recv(sd, buffer, BUFSIZE, 0);
			if(bytesIn < 0) {
				//::close(sd);   
				//client.getSocket() = 0;
			}
				 
			//Handle the commands and echo back the message that came in.
			else
			{
				using action_arguments = std::vector<std::string>;
				std::string delims = " ";	//delimater : " " and "\n".
				action_arguments arguments;
				stringstream s;
				s << buffer;
				string input_string = s.str();
				input_string.erase( std::remove(input_string.begin(), input_string.end(), '\n'), input_string.end() );
				input_string.erase( std::remove(input_string.begin(), input_string.end(), '\r'), input_string.end() );
				
				arguments = split(input_string, " \n");
				
				std::string command;
				action_arguments params;
				if(!arguments.empty()) {
					command = arguments[0];
					for (int i = 1; i < arguments.size(); ++i) {
						params.push_back(arguments[i]);
					}
				}
				
				handleCommand(command, params, client); // handle command.
			}   
		}   
	} 	
	
}

void HandleConnection::addRoom(const string &name) {
	rooms.push_back(Publisher(name));
	cout << "Rooms " << name << " created" << endl;
	cout << "*********rooms*******:" <<endl;
	for(auto room : rooms)
		cout << "-----------room:" << room.Name() << "---------- " <<endl;
}	

bool HandleConnection::isRoomCreated(const string &name) {
	auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == name;});
	return (it != rooms.end());
}
