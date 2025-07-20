#include "MasterServer.h"

Master::Master()
: _redis("tcp://127.0.0.1:6379"),
_port(PORT)
{}

