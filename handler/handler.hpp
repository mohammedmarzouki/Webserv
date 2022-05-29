#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "../webserv.hpp"


namespace sock {
    class SocketMaker {
        private:
            webserv::Server server;
            const std::vector<webserv::Location> location;
        public:
            SocketMaker(webserv::Server serv);
            ~SocketMaker();
        };
    
    
}


#endif