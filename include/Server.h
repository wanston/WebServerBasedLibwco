//
// Created by tong on 19-4-9.
//

#ifndef WEBSERVERBASEDLIBWCO_SERVER_H
#define WEBSERVERBASEDLIBWCO_SERVER_H

#include <vector>
#include <thread>
#include "Worker.h"

using namespace std;

class Server {
public:
    Server(unsigned short port, int threads, int timeout, const string& servePath):
    __port(port),__processNum(threads), __timeout(timeout), __path(servePath){}
    ~Server();
    void run();

private:
    unsigned short __port;
    int __processNum;
    int __listenFd;
    int __timeout;
    string __path;
    vector<thread> __threads;
};


#endif //WEBSERVERBASEDLIBWCO_SERVER_H
