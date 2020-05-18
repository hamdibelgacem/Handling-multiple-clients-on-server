#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <map>
#include <functional>
#include <vector>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <errno.h>

using namespace std;

class Socket
{
public:
    int sock;
    string address;
    string port;
    struct addrinfo address_info;
    Socket();
    Socket(int domain,int type,int protocol);
    int bind(string ip, string port);
    int connect(string ip, string port);
    int listen(int max_queue);
    Socket* accept();
    int socket_set_opt(int level, int optname, void* optval);
    int socket_get_opt(int level, int optname, void* optval);
    int set_blocking();
    int set_non_blocking();
    int socket_shutdown(int how);
    void close();
    static string ipFromHostName(string hostname);
};
