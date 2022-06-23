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
		std::string _content_length;
		std::string _transfer_encoding;
		int _body_fd;

	public:
		Request();

		void set_method(std::string);
		void set_path(std::string);
		void set_connection(std::string);
		void set_content_length(std::string);
		void set_transfer_encoding(std::string);
		std::string get_method(void) const;
		std::string get_path(void) const;
		std::string get_connection(void) const;
		std::string get_content_length(void) const;
		std::string get_transfer_encoding(void) const;
	};
	std::ostream &operator<<(std::ostream &, Request const &);

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

		int request_first_line(std::string, webserv::Request &);
		std::string find_value(std::string, std::string);

		void get_handle();
		void post_handle();
		void delete_handle();
	};

} // namespace webserv

#endif // HANDLE_REQUEST_HPP
