#if !defined(HANDLE_REQUEST_HPP)
#define HANDLE_REQUEST_HPP

#include "../webserv.hpp"

#define RECV_SIZE 1024

namespace webserv
{
	//////////////////////////////////////////////////
	// Request class
	//////////////////////////////////////////////////
	class Request
	{
	private:
		std::string _method;
		std::string _path;
		std::string _connection;
		int _body_fd;

	public:
		Request();
	};

	//////////////////////////////////////////////////
	// Response class
	//////////////////////////////////////////////////
	class Response
	{
	private:
	public:
		Response();
	};

	//////////////////////////////////////////////////
	// Handle_request class
	//////////////////////////////////////////////////
	class Handle_request
	{
	private:
		std::map<int, Request> requests;

	public:
		Handle_request();

		int recv_request(int, webserv::Server &);
		void treat_request(int);
		int send_response(int);
	};

} // namespace webserv

#endif // HANDLE_REQUEST_HPP
