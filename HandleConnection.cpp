#include "HandleConnection.h"


HandleConnection::HandleConnection(Socket *master) : masterSocket(master) {}

int HandleConnection::select_client() {
	struct timeval tv;
	int secondes = 10; // set timeout 10 sec
	tv.tv_sec = secondes;
	tv.tv_usec = 0;
	int max_sd = masterSocket->sock;
	FD_ZERO(&readfds); // initiailize my current set.
	FD_SET(masterSocket->sock, &readfds); //add master socket to set
	for(int i = 0; i < client_socket.size(); ++i) {
		int sd = client_socket[i]; //socket descriptor
		
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

void HandleConnection::add_client() {
	//If something happened on the master socket ,  
	//then its an incoming connection  
	if (FD_ISSET(masterSocket->sock, &readfds))   
	{   
		Socket *newSocket = masterSocket->accept();
	   
		auto it = std::find(client_socket.begin(), client_socket.end(), 0);
		if(it != client_socket.end()) // if position is empty
			*it = newSocket->sock;
		else
			client_socket.push_back(newSocket->sock);
	}	
}

void HandleConnection::split_commands(char *buffer) {

	// handle client commands
	using action_arguments = std::vector<std::string>;
	std::map<std::string, std::function<bool(action_arguments const&)> > commands {
		{"help", [&commands](action_arguments const&){
			for(auto const& pair : commands) {std::cout << pair.first << '\n';}
			return true;
		}},
		{"subscribe", [&commands](action_arguments const&){ std::cout << "subscribe" << std::endl; return true;} },
		{"unsubscribe", [&commands](action_arguments const&){ std::cout << "unsubscribe" << std::endl; return true;} },
		{"silence", [&commands](action_arguments const&){ std::cout << "silencee" << std::endl; return true;} },
		{"sine", [&commands](action_arguments const&){ std::cout << "sine" << std::endl; return true;} },
		{"echo", [&commands](action_arguments const&){ std::cout << "echo" << std::endl; return true;} }
	};
	
	ostringstream ss;
	ss << buffer;
    std::string input_string = ss.str();
    
	std::string command;
	action_arguments params;
	
	vector<string> splits;
	char delimiter = ' ';

	size_t i = 0;
	size_t pos = input_string.find(delimiter);
	command = input_string.substr(i, pos - i);
	i = pos + 1;
	pos = input_string.find(delimiter, i);
	
	while (pos != string::npos) {
		params.push_back(input_string.substr(i, pos - i));

		i = pos + 1;
		pos = input_string.find(delimiter, i);
	}

	params.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));        


	std::cout << "command:" << command << "\t";
	cout << "params: ";
	for (auto const& p: params) std::cout << " " << p;
	
	auto what = commands.find(command);
	if (what == commands.end()) {
		std::cout << "command inconnue" << std::endl;
    } else {
		auto success = (what->second)(params);
		std::cout << (success ? "success" : "failure") << std::endl;
    }
}

void HandleConnection::receiveMessage() {
	//some IO operation on some other socket 
	char buffer[BUFSIZE];  //data buffer 
	
	for (int i = 0; i < client_socket.size(); ++i)   
	{   
		int sd = client_socket[i];
			 
		if (FD_ISSET( sd , &readfds))   
		{   
			//Check if it was for closing , and also read the  
			//incoming message
			memset(buffer, 0, BUFSIZE);
			int bytesIn = recv(sd, buffer, BUFSIZE, 0);
			if(bytesIn <= 0) {
				::close(sd);   
				client_socket[i] = 0;
			}
				 
			//Echo back the message that came in  
			else 
			{
				for(int j = 0; j < client_socket.size(); ++j) {
					int outSock = client_socket[j];
					if(outSock != masterSocket->sock) {
						/*
						 * handle connection : 
						 * split commands(command + params)
						 * run commands (subscribe, unsubscribe, silnce, sine, echo)
						 *  
						 */
						 split_commands(buffer);
						 
					}
				} 
			}   
		}   
	} 	
	
}


