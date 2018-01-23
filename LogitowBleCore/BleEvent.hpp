#ifndef _EVENT_BLE_EVENT_HPP_
#define _EVENT_BLE_EVENT_HPP_

#include "event\Event.hpp"

#include <string>

class BleEvent : public Event
{
public:
	BleEvent(Object* sender, std::string const & msg) :
	Event(sender),
	msg(msg) {

	}

	virtual ~BleEvent()
	{

	}

	//获取消息
	std::string const & getMessage() {
		return msg;
	}

private:
	std::string const & msg;
};

#endif // !
