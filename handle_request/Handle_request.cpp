#include "Handle_request.hpp"

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
webserv::Request::Request()
	: _method("NULL"), _path("NULL"), _connection("NULL"), _content_length("NULL"), _transfer_encoding("NULL"), _body_fd(-1) {(void)_body_fd;}

void webserv::Request::set_method(std::string method) { this->_method = method; }
void webserv::Request::set_path(std::string path) { this->_path = path; }
void webserv::Request::set_connection(std::string connection) { this->_connection = connection; }
void webserv::Request::set_content_length(std::string content_length) { this->_content_length = content_length; }
void webserv::Request::set_transfer_encoding(std::string transfer_encoding) { this->_transfer_encoding = transfer_encoding; }
std::string webserv::Request::get_method(void) const { return _method; }
std::string webserv::Request::get_path(void) const { return _path; }
std::string webserv::Request::get_connection(void) const { return _connection; }
std::string webserv::Request::get_content_length(void) const { return _content_length; }
std::string webserv::Request::get_transfer_encoding(void) const { return _transfer_encoding; }

std::ostream &webserv::operator<<(std::ostream &output, webserv::Request const &i)
{
	output << "i.get_method():			" << i.get_method() << std::endl;
	output << "i.get_path():			" << i.get_path() << std::endl;
	output << "i.get_connection():		" << i.get_connection() << std::endl;
	output << "i.get_content_length():		" << i.get_content_length() << std::endl;
	output << "i.get_transfer_encoding():	" << i.get_transfer_encoding() << std::endl;
	return output;
}

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
							Content-Length: 13\r\n\
							Transfer-Encoding: chunked\r\n\r\n";

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
		request_first_line(received, requests[fd]);
		requests[fd].set_connection(find_value("Connection:", received));
		requests[fd].set_content_length(find_value("Content-Length:", received));
		requests[fd].set_transfer_encoding(find_value("Transfer-Encoding:", received));
		std::cout << requests[fd] << std::endl;

		// check for errors; location doesn't accept such method

		// redirection
		if (requests[fd].get_method() == "GET")
			webserv::Handle_request::get_handle();
		else if (requests[fd].get_method() == "POST")
			webserv::Handle_request::post_handle();
		else if (requests[fd].get_method() == "DELETE")
			webserv::Handle_request::delete_handle();
		else
		{
		}
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

int webserv::Handle_request::request_first_line(std::string received, webserv::Request &request)
{
	size_t pos = received.find_first_of("\r\n");
	if (pos == std::string::npos)
		return -1;
	std::vector<std::string> splitted_first_line = split_string(received.substr(0, pos), " ");
	request.set_method(splitted_first_line[0]);
	request.set_path(splitted_first_line[1]);
	return 1;
}
std::string webserv::Handle_request::find_value(std::string key, std::string received)
{
	size_t pos = received.find(key);
	std::string whole_line = received.substr(pos, received.find("\r\n", pos));
	std::vector<std::string> splitted_line = split_string(whole_line, " ");
	return splitted_line[1];
}

void webserv::Handle_request::get_handle() {}
void webserv::Handle_request::post_handle() {}
void webserv::Handle_request::delete_handle() {}
