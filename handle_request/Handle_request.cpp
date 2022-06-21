#include "Handle_request.hpp"

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
webserv::Request::Request() : _body_fd(-1) {}

//////////////////////////////////////////////////
// Response class
//////////////////////////////////////////////////
webserv::Response::Response() {}

//////////////////////////////////////////////////
// Handle_request class
//////////////////////////////////////////////////
webserv::Handle_request::Handle_request() {}

int webserv::Handle_request::recv_request(int fd, webserv::Server &server)
{
	(void)fd;
	(void)server;
	char temp[RECV_SIZE] = "POST /photos HTTP/1.1\r\n\
				 			Host: localhost:8080\r\n\
							Connection: keep-alive\r\n\
							Content-Type: application/x-www-form-urlencoded\r\n\
							Content-Length: 13";

	if (requests.find(fd) == requests.end())
		requests[fd] = Request();
	// int r = recv(fd, temp, RECV_SIZE, 0);
	int r = 1;
	std::string received(temp);
	if (r < 1)
	{
		requests.erase(fd);
		return FAILED;
	}
	else
	{
		// Parse info and fill in the requests[fd]
		// _method | _host | _connection | _content_lenght ...
		// check for errors; location doesn't accept such method

		// if _method is GET goto method_get(Request &);
		// else if _method is POST goto method_post(Request &);
		// else if _method is DELETE goto method_delete(Request &);
		return DONE;
	}
}
void webserv::Handle_request::treat_request(int fd)
{
	(void)fd;
}
int webserv::Handle_request::send_response(int fd)
{
	(void)fd;
	return DONE;
}
