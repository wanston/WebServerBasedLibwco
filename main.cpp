//
// Created by tong on 18-12-28.
//

#include <iostream>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include "Server.h"

using namespace std;

int main(int argc, char *argv[]) {
    long threads = 4;
    long port = 8080;
    long timeout = 5;
    std::string servePath = "./www";

    Server server(port, threads, timeout, servePath);

    server.run();

    return 0;
}