#if !defined(HANDLE_REQUEST_RESPONSE_HPP)
#define HANDLE_REQUEST_RESPONSE_HPP

#define PRINT_ERR(err) std::cerr << err << std::endl
#define PRINT(msg) std::cout << msg << std::endl

// header and body status
#define RECEIVE 0
#define READ 1
#define PARSED 2
#define FULLY_PARSED 3

// Request
#define FAILED -1
#define CHUNCKED 0
#define DONE 1

// Response
#define KILL_CONNECTION 0
#define KEEP_ALIVE 1

#include "../parser/Parser.hpp"
#include <sstream>
#include <map>
#include <sys/socket.h> // socket(2), accept(2), listen(2), send(2), recv(2), bind(2), connect(2), inet_addr(3), setsockopt(2), getsockname(2)

#define BUFFER_SIZE 1025

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
class Request
{
private:
	std::string _method;
	std::string _path;
	std::string _connection;
	size_t _content_length;
	std::string _transfer_encoding;
	std::string _temp_header;
	Location _location;
	short _status_code;
	short _header_status;
	short _body_status;
	size_t _read_bytes;
	std::string _path_to_upload;

public:
	Request();

	void set_method(std::string);
	void set_path(std::string);
	void set_connection(std::string);
	void set_content_length(std::string);
	void set_transfer_encoding(std::string);
	void set_temp_header(std::string);
	void set_location(Location);
	void set_status_code(short);
	void set_header_status(short);
	void set_body_status(short);
	void set_path_to_upload(std::string);
	void set_read_bytes(size_t);
	std::string get_method(void) const;
	std::string get_path(void) const;
	std::string get_connection(void) const;
	size_t get_content_length(void) const;
	std::string get_transfer_encoding(void) const;
	std::string get_temp_header(void) const;
	Location get_location(void) const;
	short get_header_status(void) const;
	short get_status_code(void) const;
	short get_body_status(void) const;
	size_t get_read_bytes(void) const;
	std::string get_path_to_upload(void) const;
};

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
	std::string _file_name;

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
// Handle_request_response class
//////////////////////////////////////////////////
class Handle_request_response
{
private:
	std::map<int, std::pair<Request, Response> > requests;

public:
	Handle_request_response();

	int recv_request(int, Server &);
	int get_handle(int, Server &);
	int post_handle(int, std::string &, int);
	int delete_handle(int, Server &);
	int send_response(int);

	int request_first_line(int, std::string, Server &);
	std::string find_value(std::string, std::string);
	Location right_location(std::string, Server &);
	Location wanted_location(std::string, Server &);
	bool is_method_allowed(Location, std::string);
	void fix_path(Request &);
	std::vector<std::string> split_string(std::string, std::string);
};

#endif // HANDLE_REQUEST_RESPONSE_HPP
