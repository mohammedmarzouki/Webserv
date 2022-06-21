#if !defined(WEBSERV_HPP)
#define WEBSERV_HPP

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// Request
#define FAILED -1
#define CHUNCKED 0
#define DONE 1

// Response
#define KILL_CONNECTION 0
#define KEEP_ALIVE 1

#define PRINT_ERR(err) std::cerr << err << std::endl
#define PRINT(msg) std::cout << msg << std::endl

#include <map>
#include <string>

#include <arpa/inet.h>	// htons(3), htonl(3), ntohs(3), ntohl(3)
#include <fcntl.h>		// fcntl(2)
#include <poll.h>		// poll(2)
// #include <sys/epoll.h> // epoll(7), epoll_create(2), epoll_ctl(2), epoll_wait(2)
#include <sys/event.h>	// kqueue(2)
#include <sys/select.h>	// select(2)
#include <sys/socket.h>	// socket(2), accept(2), listen(2), send(2), recv(2), bind(2), connect(2), inet_addr(3), setsockopt(2), getsockname(2)
#include <sys/time.h>	// kqueue(2)
#include <sys/types.h>	// kqueue(2)
#include <unistd.h>     //close(2)
#include <map>

#include "parser/Parser.hpp"
#include "handler/handler.hpp"
#include "handle_request/Handle_request.hpp"


// TOOLS
void	setup_config_file(int, char **, std::string &);
void	read_config_file(std::string &);
void	exit_err(std::string);


#endif // WEBSERV_HPP
