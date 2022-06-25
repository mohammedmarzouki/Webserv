#if !defined(HANDLE_REQUEST_HPP)
#define HANDLE_REQUEST_HPP

// Request
#define FAILED -1
#define CHUNCKED 0
#define DONE 1

// Response
#define KILL_CONNECTION 0
#define KEEP_ALIVE 1

#include <iostream>
#include <sstream>
#include <map>
#include <sys/socket.h> // socket(2), accept(2), listen(2), send(2), recv(2), bind(2), connect(2), inet_addr(3), setsockopt(2), getsockname(2)
#include "../parser/Parser.hpp"

#define RECV_SIZE 1024

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
	std::string _temp_body;
	bool _header_read;

public:
	Request();

	void set_method(std::string);
	void set_path(std::string);
	void set_connection(std::string);
	void set_content_length(std::string);
	void set_transfer_encoding(std::string);
	void set_temp_body(std::string);
	void set_header_read(bool);
	std::string get_method(void) const;
	std::string get_path(void) const;
	std::string get_connection(void) const;
	std::string get_content_length(void) const;
	std::string get_transfer_encoding(void) const;
	std::string get_temp_body(void) const;
	bool get_header_read(void) const;
};
std::ostream &operator<<(std::ostream &, Request const &);

//////////////////////////////////////////////////
// Response class
//////////////////////////////////////////////////
class Response
{
private:
	std::string _status_line;
	std::string _connection;
	std::string _content_length;
	std::string _content_type;

public:
	Response();

	void set_status_line(std::string);
	void set_connection(std::string);
	void set_content_length(std::string);
	void set_content_type(std::string);
	std::string get_status_line(void) const;
	std::string get_connection(void) const;
	std::string get_content_length(void) const;
	std::string get_content_type(void) const;

	void clear_response();
};

//////////////////////////////////////////////////
// Handle_request class
//////////////////////////////////////////////////
class Handle_request
{
private:
	std::map<int, std::pair<Request, Response> > requests;

public:
	Handle_request();

	int recv_request(int, Server &);
	int treat_request(int);
	int send_response(int);

	int request_first_line(std::string, Request &);
	std::string find_value(std::string, std::string);
	Location wanted_location(std::string, Server &);

	void get_handle();
	void post_handle();
	void delete_handle();
};

//////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////
std::vector<std::string> split_string(std::string, std::string);

#endif // HANDLE_REQUEST_HPP
