#pragma once

#include "include.h"

inline const char* purple = "\033[32m";    
inline const char* brightMagenta = "\033[37m"; 
inline const char* reset = "\033[0m";

class auth_Page {
public:
    static void Start();
    
};

class friend_Page {
public:
    static void Friend();
    static void Friend_detil();
    static void Group_detil();
};

class chat_Page {
public:
    static void Chat();
};