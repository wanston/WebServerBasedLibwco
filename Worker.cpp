//
// Created by tong on 19-4-9.
//

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <unistd.h>
#include <memory>
#include <arpa/inet.h>
#include "Worker.h"

extern "C"{
#include "wco_routine.h"
#include "wco_scheduler.h"
#include "wco_hook_sys_call.h"
}

Worker::Worker(int fd) : __fd(fd) {
    __parser.callback = std::bind<bool>(&Worker::__parserCallback, this, _1, _2, _3, _4, _5);
}


void Worker::work() {
    ssize_t readRet, writeRet;
    size_t sz = 1024;
    char* buf = new char[sz];

    while((readRet = read(__fd, buf, sz)) > 0){

        bool stop = false;
        if(__parser(buf, readRet) < 0){
            vector<char> response = Response::make_xxx_response(BAD_REQUEST);
            __sendBuf.insert(__sendBuf.end(), response.begin(), response.end());
            stop = true;
        }

        if(!__sendBuf.empty()){
            size_t total = 0, target = __sendBuf.size();
            while(total < target && (writeRet = write(__fd, __sendBuf.data()+total, __sendBuf.size()-total)) > 0){
                total += writeRet;
            }
            __sendBuf.erase(__sendBuf.begin(), __sendBuf.begin()+total);
            if(total < target){
                stop = true;
            }
        }

        if(stop){
            break;
        }
    }

    struct sockaddr_in clientAddr = {0};
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(__fd, (struct sockaddr *)&clientAddr, &addr_size);

    close(__fd);
    delete[](buf);
}


bool Worker::__parserCallback(string &method, string &url, string &version,
        std::vector<Header> &requestHeaders, std::vector<char> &messageBody)
{
    vector<char> response;

    if(version != "HTTP/1.1"){
        response = std::move(Response::make_xxx_response(HTTP_VERSION_NOT_SUPPORTED));
    }else{
        if(method == "GET"){
            response = Response::make_get_response(url, requestHeaders, messageBody);
        }else if(method == "POST"){
            response = std::move(Response::make_post_response(url, requestHeaders, messageBody));
        }else if(method == "HEAD"){
            response = std::move(Response::make_head_response(url, requestHeaders, messageBody));
        }else{
            response = std::move(Response::make_xxx_response(METHOD_NOT_ALLOWD));
        }
    }

    __sendBuf.insert(__sendBuf.end(), response.begin(), response.end());

    bool keepGoing = false;
    return keepGoing;
}




