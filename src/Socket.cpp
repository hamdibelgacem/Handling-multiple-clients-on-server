#include "Socket.h"

Socket::Socket(){}

Socket::Socket(int domain, int type, int protocol)
{
    memset(&address_info, 0, sizeof address_info);
    sock = socket(domain, type , protocol);
    if (sock < 0) {
        //exit(1);
        cerr << "opening socket error: " << gai_strerror(errno) << endl;
    }
    address_info.ai_family = domain;
    address_info.ai_socktype = type;
    address_info.ai_protocol = protocol;
    
    port = "";
    address = "";
}

int Socket::bind(string ip, string port){
	
	//check if port number is type int
	for(int i = 0; i < port.length(); i++) 
	{
		if (isdigit(port[i]) == false) {
			std::cerr << "The port number must be an integer" << endl;
			exit(EXIT_FAILURE);
		}
	}
	
    if (address_info.ai_family == AF_UNIX) {    
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, ip.c_str(), sizeof(addr.sun_path)-1);
        int status = ::bind(sock, (struct sockaddr*)&addr, sizeof(addr));
        if (status < 0) {
            cerr << "bind error: " << gai_strerror(errno) << endl;
        }
        return status;
    }
    
    address = ip;
    this->port = port;
    int status;
    struct addrinfo *res;
    address_info.ai_flags = AI_PASSIVE;
    if ((status = getaddrinfo(ip.c_str(), port.c_str(), &address_info, &res)) != 0) {
        cerr << "getaddrinfo error: " << gai_strerror(errno) << endl;
        //exit(1);
        return status;
    }
    address_info.ai_addrlen = res->ai_addrlen;
    address_info.ai_addr = res->ai_addr;
    freeaddrinfo(res);
    status = ::bind(sock, address_info.ai_addr, address_info.ai_addrlen);
    if (status < 0) {
        //exit(1);
        cerr << "bind error: " << gai_strerror(errno) << endl;
    }
    return status;
}

int Socket::connect(string ip, string port){
    if (address_info.ai_family == AF_UNIX) {
        
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, ip.c_str(), sizeof(addr.sun_path)-1);
        int status = ::connect(sock, (struct sockaddr*)&addr, sizeof(addr));
        if (status < 0) {
            //exit(1);
            cerr << "connect error: " << gai_strerror(errno) << endl;
        }
        return status;
    }
      
    address = ip;
    this->port = port;
    struct addrinfo *res;
    int status;
    if ((status = getaddrinfo(ip.c_str(), port.c_str(), &address_info, &res)) != 0){
        cerr << "getaddrinfo error: " << gai_strerror(errno);
        //exit(1);
        return status;
    }
    address_info.ai_addrlen = res->ai_addrlen;
    address_info.ai_addr = res->ai_addr;
    freeaddrinfo(res);
    status = ::connect(sock, address_info.ai_addr, address_info.ai_addrlen);
    if (status < 0) {
        //exit(1);
        cerr << "connect error: " << gai_strerror(status) << endl;
    }
    return status;
}

int Socket::listen(int max_queue){
    int status;
    status = ::listen(sock,max_queue);
    if (status < 0) {
        //exit(1);
        cerr << "listen error: " << gai_strerror(errno) << endl;
    }
    cout << "Listener on port " << port << endl;
    return status;
}

Socket* Socket::accept(){
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    addr_size = sizeof their_addr;
    int newsock = ::accept(sock, (struct sockaddr *)&their_addr, &addr_size);
    if (newsock < 0) {
        //exit(1);
        cerr << "accept error: " << gai_strerror(errno) << endl;
    }

    Socket *newSocket = new Socket(address_info.ai_family,address_info.ai_socktype,address_info.ai_protocol);
    newSocket->sock = newsock;
    newSocket->port = port;
    
    char host[NI_MAXHOST];
    int status = getnameinfo((struct sockaddr *)&their_addr, sizeof(their_addr), host, sizeof(host), NULL, 0, NI_NUMERICHOST);
    if (status < 0) {
        //exit(1);
        cerr << "getnameinfo error: " << gai_strerror(errno) << endl;
    }
    newSocket->address = host;
    newSocket->address_info.ai_family = their_addr.ss_family;
    newSocket->address_info.ai_addr = (struct sockaddr *)&their_addr;

    //send new connection greeting message
    #if 0
    string welcomeMsg = "Welcome to chat room!\r\n"; 
    send(newsock, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
    cout << "Welcome message sent successfully" << endl;
    #endif
    
    return newSocket;
}

int Socket::socket_set_opt(int level, int optname, void* optval){
    unsigned int len = sizeof(optval);
    int status = ::setsockopt(sock,level,optname,optval,len);
    if (status < 0) {
        //exit(1);
        cerr << "socket_set_opt error: " << gai_strerror(errno) << endl;
    }
    return status;
}

int Socket::socket_get_opt(int level, int optname, void* optval){
    unsigned int len = sizeof(optval);
    int status = ::getsockopt(sock,level,optname,optval,&len);
    if (status < 0) {
        //exit(1);
        cerr << "socket_get_opt error: " << gai_strerror(errno) << endl;
    }
    return status;
}

int Socket::set_blocking(){
    long status = fcntl(sock, F_GETFL, NULL);
    if (status < 0) {
        //exit(1);
        cerr << "set_blocking(get) error: " << gai_strerror(errno) << endl;
    }
    status &= (~O_NONBLOCK);
    status = fcntl(sock, F_SETFL, status);
    if (status < 0) {
        //exit(1);
        cerr << "set_blocking(set) error: " << gai_strerror(errno) << endl;
    }
    return (int)status;
}

int Socket::set_non_blocking(){
    long status = fcntl(sock, F_GETFL, NULL);
    if (status < 0) {
        //exit(1);
        cerr << "set_non_blocking(get) error: " << gai_strerror(errno) << endl;
    }
    status |= O_NONBLOCK;
    status = fcntl(sock, F_SETFL, status);
    if (status < 0) {
        //exit(1);
        cerr << "set_non_blocking(set) error: " << gai_strerror(errno) << endl;
    }
    return (int)status;
}

int Socket::socket_shutdown(int how){
    int status = ::shutdown(sock, how);
    if (status < 0) {
        //exit(1);
        cerr << "shutdown error: " << gai_strerror(errno) << endl;
    }
    return status;
}

void Socket::close(){
    ::close(sock);
}
