//
// Created by tong on 19-4-9.
//

#ifndef WEBSERVERBASEDLIBWCO_WORKER_H
#define WEBSERVERBASEDLIBWCO_WORKER_H

#include <string>
#include "HttpParser.h"
#include "Response.h"

using namespace std;
using namespace std::placeholders;

class Worker {
    int __fd;
    HttpRequestParser __parser;
    vector<char> __sendBuf;
    bool __parserCallback(std::string &, std::string &, std::string &, std::vector<Header> &, std::vector<char> &);

public:
    Worker(int fd);
    void work();
};

#endif //WEBSERVERBASEDLIBWCO_WORKER_H
