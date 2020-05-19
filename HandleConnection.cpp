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
		int sd = client.get_socket(); //socket descriptor
		
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
		it = find_if(it, clients.end(), [=](Subscriber s) { return s.get_socket() == 0; } );
		// remove client
		// todo : unsubscribe this client from the subscriber rooms
		clients.erase(it);
		it++;
	}
}

void HandleConnection::addClient() {
	//If something happened on the master socket ,  
	//then its an incoming connection  
	if (FD_ISSET(masterSocket->sock, &readfds))   
	{   
		Socket *newSocket = masterSocket->accept();
		// Adding client to Clients list.
		int socket = newSocket->sock;
		Subscriber subscriber(socket);
		clients.push_back(subscriber);
		cout << "after push sub : " << endl;
	}	
}

void HandleConnection::subscribe(const std::vector<string> &params, Subscriber &client) {
	cout << "HandleConnection::subscribe" << endl;
	for(const auto & param : params) {
		if(!isRoomCreated(param)) {
			cout << "------- room " << param << " is not created ------ " << param << endl;
			addRoom(param);
		} else {
			cout<<"------- Room "<< param << " is created -------- "<<param<<endl;
		}
		
		auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == param;} );
		if(it != rooms.end())
			Subscribe(*it, client);
	}
}

void HandleConnection::echo(const std::vector<string> &params) {
	auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == params[0];} );
	it->Publish("12");
}

void HandleConnection::handleCommand(const string &command, const std::vector<string> &params, Subscriber &client) {
	/*std::map<std::string, std::function<bool(action_arguments const&)> > commands {
		{"help", [&commands](action_arguments const&){
			for(auto const& pair : commands) {std::cout << pair.first << '\n';}
			return true;
		}},
		{"subscribe", [&commands, params, client, this](action_arguments const&) { this->subscribe(params, client); return true; } },
		{"unsubscribe", [&commands](action_arguments const&){ std::cout << "run unsubscribe" << std::endl; return true;} },
		{"silence", [&commands](action_arguments const&){ std::cout << "run silencee" << std::endl; return true;} },
		{"sine", [&commands](action_arguments const&){ std::cout << "run sine" << std::endl; return true;} },
		{"echo", [&commands, params, this](action_arguments const&){ this->echo(params); return true;} }
	};*/
	
	if(command == "subscribe") {
		subscribe(params, client);
	}
	else if (command == "echo") {
		echo(params);
	}
	/*
	auto what = commands.find(command);
	if (what == commands.end()) {
		std::cout << "command inconnue" << std::endl;
    } else {
		auto success = (what->second)(params);
		std::cout << (success ? "success" : "failure") << std::endl;
    }*/
}

void HandleConnection::receiveMessage() {
	//some IO operation on some other socket 
	char buffer[BUFSIZE];  //data buffer 
	
	for (auto &client : clients)   
	{   
		int sd = client.get_socket();
			 
		if (FD_ISSET( sd , &readfds))   
		{   
			//Check if it was for closing , and also read the  
			//incoming message
			memset(buffer, 0, BUFSIZE);
			int bytesIn = recv(sd, buffer, BUFSIZE, 0);
			if(bytesIn <= 0) {
				//::close(sd);   
				//client.get_socket() = 0;
			}
				 
			//Echo back the message that came in  
			else 
			{
				int outSock = client.get_socket();
					/*
					 * handle connection : 
					 * split commands(command + params)
					 * run commands (subscribe, unsubscribe, silnce, sine, echo)
					 *  
					 */
				
					using action_arguments = std::vector<std::string>;
					
					std::string input_string(buffer);
					std::string delims = " \n";
					action_arguments arguments = split(input_string, delims);
					
					std::string command = arguments[0];
					action_arguments params;
		
					for (int i = 1; i < arguments.size(); ++i) {
						params.push_back(arguments[i]);
					}
					
					handleCommand(command, params, client); // handle is valid command ?
				
			}   
		}   
	} 	
	
}

void HandleConnection::addRoom(const string &name) {
	rooms.push_back(Publisher(name));
	cout << "Rooms " << name << " created" << endl;
	cout << "*********rooms*******:" <<endl;
	for(auto room : rooms)
		cout << "-----------room : " << room.Name() << " ---------- " <<endl;
}	

bool HandleConnection::isRoomCreated(const string &name) {
	cout << "---------- check room " << name << " is created " << endl;
	auto it = find_if(rooms.begin(), rooms.end(), [=](Publisher p){ return p.Name() == name;});
	return (it != rooms.end());
}
