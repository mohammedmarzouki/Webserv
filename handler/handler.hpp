#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "../webserv.hpp"


namespace sock {
    class SocketMaker {
        public:
            int _socket;
            webserv::Server server;
            const std::vector<webserv::Location> location;
            SocketMaker(webserv::Server serv);
            ~SocketMaker();
        };
    
    
}


#endif