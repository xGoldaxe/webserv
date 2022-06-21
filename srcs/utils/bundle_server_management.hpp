#pragma once

#include "../configuration/bundle_server.hpp"

std::vector<Bundle_server> pack_servers(std::vector<Server_conf> servers);
void print_bundled_servers(std::vector<Bundle_server> bundle_servers);
