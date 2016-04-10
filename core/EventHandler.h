#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

namespace sms {
namespace event {

template < typename Handler >
class EventHandler {
public:
    EventHandler();
private:
};

template < typename Handler >
EventHandler::EventHandler() {

}

}
}

#endif // EVENTHANDLER_H
