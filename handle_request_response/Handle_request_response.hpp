#if !defined(HANDLE_REQUEST_RESPONSE_HPP)
#define HANDLE_REQUEST_RESPONSE_HPP

#define PRINT_ERR(err) std::cerr << err << std::endl
#define PRINT(msg) std::cout << msg << std::endl

// Status Codes
/// 1xx Informational
#define CONTINUE 100
#define SWITCHING_PROTOCOLS 101
/// 2xx Succesful
#define OK 200
#define CREATED 201
#define ACCEPTED 202
#define NON_AUTHORITATIVE_INFORMATION 203
#define NO_CONTENT 204
#define RESET_CONTENT 205
#define PARTIAL_CONTENT 206
/// 3xx Redirection
#define MULTIPLE_CHOICES 300
#define MOVED_PERMANENTLY 301
#define FOUND 302
#define SEE_OTHER 303
#define NOT_MODIFIED 304
#define USE_PROXY 305
#define TEMPORARY_REDIRECT 307
#define PERMANENT_REDIRECT 308
/// 4xx Client Error
#define BAD_REQUEST 400
#define UNAUTHORIZED 401
#define PAYMENT_REQUIRED 402
#define FORBIDDEN 403
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405
#define NOT_ACCEPTABLE 406
#define PROXY_AUTHENTICATION_REQUIRED 407
#define REQUEST_TIMEOUT 408
#define CONFLICT 409
#define GONE 410
#define LENGTH_REQUIRED 411
#define PRECONDITION_FAILED 412
#define PAYLOAD_TOO_LARGE 413
#define REQUEST_URI_TOO_LONG 414
#define UNSUPPORTED_MEDIA_TYPE 415
#define REQUESTED_RANGE_NOT_SATISFIABLE 416
#define EXPECTATION_FAILED 417
/// 5xx Server Error
#define INTERNAL_SERVER_ERROR 500
#define NOT_IMPLEMENTED 501
#define BAD_GATEWAY 502
#define SERVICE_UNAVAILABLE 503
#define GATEWAY_TIMEOUT 504
#define HTTP_VERSION_NOT_SUPPORTED 504

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
#define HEADER_NOT_SENT 0
#define HEADER_SENT 1
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
	std::string get_method() const;
	std::string get_path() const;
	std::string get_connection() const;
	size_t get_content_length() const;
	std::string get_transfer_encoding() const;
	std::string get_temp_header() const;
	Location get_location() const;
	short get_header_status() const;
	short get_status_code() const;
	short get_body_status() const;
	size_t get_read_bytes() const;
	std::string get_path_to_upload() const;
};

//////////////////////////////////////////////////
// Handle_request_response class
//////////////////////////////////////////////////
class Response
{
private:
	std::string _header;
	bool _header_sent;
	unsigned long _bytes_sent;
	unsigned long _content_length;

public:
	Response();

	void set_header(std::string);
	void set_header_sent(bool);
	void set_bytes_sent(unsigned long);
	void set_content_length(unsigned long);
	std::string get_header() const;
	bool get_header_sent() const;
	unsigned long get_bytes_sent() const;
	unsigned long get_content_length() const;

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

	std::string header_maker(short);
	std::string status_line_maker(short);
	std::string content_type_maker(std::string);
	std::string ext_from_path(std::string);
	std::string to_string(int);
};

#endif // HANDLE_REQUEST_RESPONSE_HPP
