#pragma once

#include <iostream>
#include <string>
#include <sstream> 
#include <algorithm>
#include <map>
#include <vector>

#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;


class EventArgs {
public:
	virtual ~EventArgs();
};

class StringEventArgs : public EventArgs {
	std::vector<uint16_t> payload_;
public:
	explicit StringEventArgs(const std::vector<uint16_t>& payload);
	const std::vector<uint16_t> Payload() const;
};

class Event {
	class Callback {
		void* pSender_;
		const EventArgs& args_;

	public:
		Callback(void* pSender, const EventArgs& args) : pSender_(pSender), args_(args) {}
		void operator()(pair<long, function<void(void*, const EventArgs&)>> p) const {
			p.second(pSender_, args_);
		}
	};
	map<long, function<void(void*, const EventArgs&)>> callbacks_;
	long token_ = 0;

public:
	void operator()(void* pSender, const EventArgs& args) const;
	long Subscribe(function<void(void*, const EventArgs&)> f);
	void Unsubscribe(long token);
};

class Publisher {
	Event event_;
	string name_;

public:
	explicit Publisher(const string& name);
	const string& Name() const;
	void Publish(const std::vector<uint16_t>& message);
	long Register(function<void(void*, const EventArgs&)> f);
	void Unregister(long token);
};

class Subscriber {
	int socket_;
public:
	explicit Subscriber(const int& socket);
	int getSocket() const;
	void OnEventReceived(void* pSender, const EventArgs& args);
};

namespace PubSub {
	using namespace std::placeholders;
	long Subscribe(Publisher& publisher, Subscriber& subscriber);
	void Unsubscribe(Publisher& publisher, long token);
}

class Subscription {
	Publisher& publisher_;
	long token_;
public:
	Subscription(Publisher& publisher, long token);
	~Subscription();
};
