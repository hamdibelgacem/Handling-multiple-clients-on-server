#include "pub-sub.h"
#include <arpa/inet.h>

EventArgs::~EventArgs() {}

StringEventArgs::StringEventArgs(const std::vector<uint16_t>& payload) : payload_(payload){}

const std::vector<uint16_t> StringEventArgs::Payload() const { return payload_; }


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

void Publisher::Publish(const std::vector<uint16_t>& message) {
	event_(this, StringEventArgs(message));
}

long Publisher::Register(function<void(void*, const EventArgs&)> f) {
	return event_.Subscribe(f);
}

void Publisher::Unregister(long token) {
	event_.Unsubscribe(token);
}

Subscriber::Subscriber(const int& socket) : socket_(socket){}
int Subscriber::getSocket() const {return socket_;}
void Subscriber::OnEventReceived(void* pSender, const EventArgs& args) {
	const StringEventArgs* const s = dynamic_cast<const StringEventArgs* const>(&args);
	if (s == nullptr)
		return;
	if (pSender == nullptr)
		return;
	//Publisher* p = static_cast<Publisher*>(pSender);

	std::vector<uint16_t> buffer = s->Payload();
	
	for(auto &elem : buffer)
	{
		elem = htons(elem);
	}
		
	send(socket_, &buffer[0], buffer.size() * sizeof(uint16_t), 0);
}

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
