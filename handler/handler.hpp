#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "../webserv.hpp"


namespace sock {
    class SocketMaker {
        public:
            int _socket;
            webserv::Server &server;
            const std::vector<webserv::Location> location;
            SocketMaker(webserv::Server &serv);
            ~SocketMaker();
        };
    std::vector<sock::SocketMaker> srv;
    webserv::Handle_request handler;
    std::map<int,int> cli_srv;

    void looper(std::vector<webserv::Server> servers);
    int  set_servers(fd_set &rd,fd_set &wr);
    void ReadyToRead(int &fd, fd_set &rd,fd_set &wr, int &max_fd);
    void ReadyToWrite(int &fd, fd_set &rd,fd_set &wr, int &max_fd);
    bool isserver(const int &fd);
    webserv::Server &search_server(const int &fd);
}

#endif