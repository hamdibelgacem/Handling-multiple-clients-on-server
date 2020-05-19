#include "pub-sub.h"
 
EventArgs::~EventArgs() {}

StringEventArgs::StringEventArgs(const string& payload) : payload_(payload) {}
const string& StringEventArgs::Payload() const { return payload_; }

/*
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
	void operator()(void* pSender, const EventArgs& args) const {
		for_each(callbacks_.begin(), callbacks_.end(), Callback(pSender, args));
	}
	long Subscribe(function<void(void*, const EventArgs&)> f) {
		token_++;
		callbacks_.insert(make_pair(token_, f));
		return token_;
	}
	void Unsubscribe(long token) {
		callbacks_.erase(token);
	}
};
*/

/*
Event::Callback::Callback(void* pSender, const EventArgs& args) : pSender_(pSender), args_(args) {}

void Event::Callback::operator()(pair<long, function<void(void*, const EventArgs&)>> p) const {
	p.second(pSender_, args_);
}
*/

void Event::operator ()(void* pSender, const EventArgs& args) const {
	for_each(callbacks_.begin(), callbacks_.end(), Callback(pSender, args));
}

long Event::Subscribe(function<void(void*, const EventArgs&)> f) {
	token_++;
	callbacks_.insert(make_pair(token_, f));
	return token_;
}

void Event::Unsubscribe(long token) {
	callbacks_.erase(token);
}

Publisher::Publisher(const string& name) : name_(name) {}

const string& Publisher::Name() const { return name_; }

void Publisher::Publish(const string& message) {
	event_(this, StringEventArgs(message));
}

long Publisher::Register(function<void(void*, const EventArgs&)> f) {
	return event_.Subscribe(f);
}

void Publisher::Unregister(long token) {
	event_.Unsubscribe(token);
}

/*
class Publisher {
	Event event_;
	string name_;

public:
	explicit Publisher(const string& name) : name_(name) {}
	const string& Name() const { return name_; }
	void Publish(const string& message) {
		event_(this, StringEventArgs(message));
	}
	long Register(function<void(void*, const EventArgs&)> f) {
		return event_.Subscribe(f);
	}
	void Unregister(long token) {
		event_.Unsubscribe(token);
	}
};
*/

Subscriber::Subscriber(const int& socket) : socket_(socket){}
int Subscriber::getSocket() const {return socket_;}
void Subscriber::OnEventReceived(void* pSender, const EventArgs& args) {
	const StringEventArgs* const s = dynamic_cast<const StringEventArgs* const>(&args);
	if (s == nullptr)
		return;
	if (pSender == nullptr)
		return;
	Publisher* p = static_cast<Publisher*>(pSender);
	cout << "Subscriber with Socket ID=" << socket_ << " has received " << s->Payload().c_str() << " from " << p->Name().c_str() << endl;
	send(socket_, s->Payload().c_str(), s->Payload().size() + 1, 0);
}
/*
class Subscriber {
	int socket_;
public:
	explicit Subscriber(const int& socket) : socket_(socket){}
	int getSocket() const {return socket_;}
	void OnEventReceived(void* pSender, const EventArgs& args) {
		const StringEventArgs* const s = dynamic_cast<const StringEventArgs* const>(&args);
		if (s == nullptr)
			return;
		if (pSender == nullptr)
			return;
		Publisher* p = static_cast<Publisher*>(pSender);
		cout << "Subscriber with Socket ID=" << socket_ << " has received " << s->Payload().c_str() << " from " << p->Name().c_str() << endl;
		send(socket_, s->Payload().c_str(), s->Payload().size() + 1, 0);
	}
};
*/

namespace PubSub {
	using namespace std::placeholders;
	long Subscribe(Publisher& publisher, Subscriber& subscriber) {
		return publisher.Register(bind(&Subscriber::OnEventReceived, &subscriber, _1, _2));
	}
	void Unsubscribe(Publisher& publisher, long token) {
		publisher.Unregister(token);
	}
}

Subscription::Subscription(Publisher& publisher, long token) : publisher_(publisher), token_(token) {}
Subscription::~Subscription() {
		publisher_.Unregister(token_);
}
