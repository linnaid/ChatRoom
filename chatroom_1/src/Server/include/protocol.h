#pragma once

#include "include.h"

class Protocol {
public:
    static std::string pack(const std::string& payload);
    
    static std::string unpack(std::string& buffer);

    static bool is_heartbeat(const std::string& msg);
};
