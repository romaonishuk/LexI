//
// Created by romaonishuk on 04.03.20.
//

#ifndef LEXI_I_EVENT_HPP
#define LEXI_I_EVENT_HPP

#include "types.hpp

class IEvent
{
public:
    virtual ~IEvent() = default;
private:
    EventType mType;
};


#endif //LEXI_I_EVENT_HPP
