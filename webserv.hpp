#if !defined(WEBSERV_HPP)
#define WEBSERV_HPP

#define PRINT_ERR(err)  std::cerr << err << std::endl
#define PRINT(msg)  std::cout << msg << std::endl


#include <arpa/inet.h> // htons(3), htonl(3), ntohs(3), ntohl(3)
#include <fcntl.h>     // fcntl(2)
#include <poll.h>      // poll(2)
// #include <sys/epoll.h> // epoll(7), epoll_create(2), epoll_ctl(2), epoll_wait(2)
#include <sys/event.h> // kqueue(2)
#include <sys/select.h> // select(2)
#include <sys/socket.h> // socket(2), accept(2), listen(2), send(2), recv(2), bind(2), connect(2), inet_addr(3), setsockopt(2), getsockname(2)
#include <sys/time.h>   // kqueue(2)
#include <sys/types.h>  // kqueue(2)

#include "parser/Parser.hpp"

#endif // WEBSERV_HPP

