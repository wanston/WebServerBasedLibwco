//
// Created by tong on 19-4-9.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include "Server.h"
#include "Worker.h"
#include "Response.h"
extern "C"{
#include "wco_routine.h"
#include "wco_scheduler.h"
#include "wco_hook_sys_call.h"
}

bool running = true;

struct Arg{
    int fd;
    int timeout;
};

void acceptRoutine(void* arg);
void ioRoutine(void* arg);

void Server::run() {
    Response::setWorkDir(__path);
    WcoSetHookEnabled(true);

    __listenFd = socket(AF_INET,SOCK_STREAM, 0);
    int nReuseAddr = 1;
    setsockopt(__listenFd,SOL_SOCKET,SO_REUSEADDR,&nReuseAddr,sizeof(nReuseAddr));

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(__port);
    assert(bind(__listenFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0);

    int max_listen_num = 2048;
    listen(__listenFd, max_listen_num);
    assert(listen(__listenFd, max_listen_num) == 0);

    for(int i=0; i<__processNum; i++){
        __threads.emplace_back([this](){
            Arg arg = {__listenFd, __timeout};
            WcoRoutine *mainCo = WcoCreate(nullptr, acceptRoutine, &arg);
            WcoAddCoToScheduler(WcoGetScheduler(), mainCo);
            WcoRunScheduler(WcoGetScheduler());
            WcoDestroyScheduler(WcoGetScheduler());
        });
    }
}


Server::~Server() {
    for(auto& t : __threads){
        t.join();
    }
}

void acceptRoutine(void* arg){
    Arg* p = (Arg*)arg;
    int listenFd = p->fd;

    int fd = 0;
    struct sockaddr_in clientAddr = {0};
    socklen_t clientSize = sizeof(clientAddr);
    while(running){
        if((fd = accept(listenFd, (struct sockaddr *) &clientAddr, &clientSize)) >= 0) {

            struct timeval t = {p->timeout, 0};
            setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(struct timeval));
            WcoRoutine *co = WcoCreate(nullptr, ioRoutine, reinterpret_cast<void*>(fd));
            WcoAddCoToScheduler(WcoGetScheduler(), co);
        }
    }
}


void ioRoutine(void* arg){
    int fd = (int)reinterpret_cast<long>(arg);
    shared_ptr<Worker> pworker = make_shared<Worker>(fd);
    pworker->work();
}
