#include "Handle_request.hpp"

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
Request::Request()
{
	_method = "NULL";
	_path = "NULL";
	_connection = "NULL";
	_content_length = "NULL";
	_transfer_encoding = "NULL";
	_header_read = false;
}

void Request::set_method(std::string method) { this->_method = method; }
void Request::set_path(std::string path) { this->_path = path; }
void Request::set_connection(std::string connection) { this->_connection = connection; }
void Request::set_content_length(std::string content_length) { this->_content_length = content_length; }
void Request::set_transfer_encoding(std::string transfer_encoding) { this->_transfer_encoding = transfer_encoding; }
void Request::set_temp_body(std::string temp_body) { this->_temp_body = temp_body; }
void Request::set_header_read(bool header_read) { this->_header_read = header_read; }
std::string Request::get_method(void) const { return _method; }
std::string Request::get_path(void) const { return _path; }
std::string Request::get_connection(void) const { return _connection; }
std::string Request::get_content_length(void) const { return _content_length; }
std::string Request::get_transfer_encoding(void) const { return _transfer_encoding; }
std::string Request::get_temp_body(void) const { return _temp_body; }
bool Request::get_header_read(void) const { return _header_read; }

std::ostream &operator<<(std::ostream &output, Request const &i)
{
	output << "i.get_method():			" << i.get_method() << std::endl;
	output << "i.get_path():			" << i.get_path() << std::endl;
	output << "i.get_connection():		" << i.get_connection() << std::endl;
	output << "i.get_content_length():		" << i.get_content_length() << std::endl;
	output << "i.get_transfer_encoding():	" << i.get_transfer_encoding() << std::endl;
	output << "i.get_temp_body():		" << i.get_temp_body() << std::endl;
	return output;
}

//////////////////////////////////////////////////
// Response class
//////////////////////////////////////////////////
Response::Response() : _status_line("HTTP/1.1"), _connection("NULL"), _content_length("NULL"), _content_type("NULL") {}

void Response::set_status_line(std::string status_line) { this->_status_line = status_line; }
void Response::set_connection(std::string connection) { this->_connection = connection; }
void Response::set_content_length(std::string content_length) { this->_content_length = content_length; }
void Response::set_content_type(std::string content_type) { this->_content_type = content_type; }
std::string Response::get_status_line(void) const { return _status_line; }
std::string Response::get_connection(void) const { return _connection; }
std::string Response::get_content_length(void) const { return _content_length; }
std::string Response::get_content_type(void) const { return _content_type; }

void Response::clear_response()
{
	_status_line = "HTTP/1.1";
	_connection = "NULL";
	_content_length = "NULL";
	_content_type = "NULL";
}

//////////////////////////////////////////////////
// Handle_request class
//////////////////////////////////////////////////
Handle_request::Handle_request() {}

int Handle_request::recv_request(int fd, Server &server)
{
	char temp[RECV_SIZE];

	/// if server doesn't exist, add it to the map
	if (requests.find(fd) == requests.end())
	{
		requests[fd].first = Request();
		requests[fd].second = Response();
	}

	int r = recv(fd, temp, RECV_SIZE, 0);
	std::string received(temp);

	if (r < 1)
	{
		requests.erase(fd);
		return FAILED;
	}
	else
	{
		/// check if header is read
		size_t header_end = received.find("\r\n\r\n");
		if (!requests[fd].first.get_header_read() && header_end != std::string::npos)
		{
			requests[fd].first.set_header_read(true);
			requests[fd].first.set_temp_body(received.substr(header_end + 4));
		}

		if (requests[fd].first.get_header_read())
		{
			request_first_line(received, requests[fd].first);
			requests[fd].first.set_connection(find_value("Connection:", received));
			requests[fd].first.set_content_length(find_value("Content-Length:", received));
			requests[fd].first.set_transfer_encoding(find_value("Transfer-Encoding:", received));
			// std::cout << requests[fd].first;
			// std::cout << requests[fd].first.get_temp_body() << std::endl;

			// this block belongs to the below else block
			{
				if (requests[fd].first.get_method() == "POST")
				{
					// Bring right location
					Location location;
					std::string current_path = requests[fd].first.get_path();

					do
					{
						location = wanted_location(current_path, server);

						if (location.get_uri() == "NULL")
						{
							size_t pos = current_path.find_last_of("/");
							if (pos != std::string::npos)
								current_path = current_path.substr(0, pos);
						}
						else
							break;
					} while (current_path.size());

					// if location isn't found, use root location
					if (location.get_uri() == "NULL")
						location = wanted_location("/", server);

					// Check if location accept POST method before reading the body
					std::vector<std::string> allow_methods = location.get_allow_methods();
					if (!std::count(allow_methods.begin(), allow_methods.end(), "POST"))
						return FAILED;

					// Change Request path to the right path based on location root
					size_t from_path = location.get_uri().size() == 1 ? 0 : location.get_uri().size();
					requests[fd].first.set_path(location.get_root() + requests[fd].first.get_path().substr(from_path));
				}
			}
		}
		else
		{
		}

		return treat_request(fd);
	}
}
int Handle_request::treat_request(int fd)
{
	if (requests[fd].first.get_method() == "GET")
		Handle_request::get_handle();
	else if (requests[fd].first.get_method() == "POST")
		Handle_request::post_handle();
	else if (requests[fd].first.get_method() == "DELETE")
		Handle_request::delete_handle();
	else
	{
	}
	return DONE;
}
int Handle_request::send_response(int fd)
{
	char buffer[RECV_SIZE];
	std::string header;

	send(fd, buffer, strlen(buffer), 0);
	return DONE;
}

int Handle_request::request_first_line(std::string received, Request &request)
{
	size_t pos;
	size_t end_pos;

	if ((pos = received.find("GET")) == std::string::npos)
		if ((pos = received.find("POST")) == std::string::npos)
			if ((pos = received.find("DELETE")) == std::string::npos)
				return -1;
	if ((end_pos = received.find("\r\n", pos)) == std::string::npos)
		return -1;

	std::vector<std::string> splitted_first_line = split_string(received.substr(pos, end_pos - pos), " ");
	request.set_method(splitted_first_line[0]);
	request.set_path(splitted_first_line[1]);
	return 1;
}
std::string Handle_request::find_value(std::string key, std::string received)
{
	size_t pos = received.find(key);
	size_t end_pos = received.find("\r\n", pos);
	std::string whole_line;

	try
	{
		whole_line = received.substr(pos, end_pos - pos);
	}
	catch (std::out_of_range &e)
	{
		return "NULL";
	}
	std::vector<std::string> splitted_line = split_string(whole_line, " ");
	return splitted_line[1];
}
Location Handle_request::wanted_location(std::string path, Server &server)
{
	std::vector<Location> locations = server.get_locations();
	std::vector<Location>::iterator it = locations.begin();

	while (it != locations.end())
	{
		if (it->get_uri() == path)
			return *it;
		it++;
	}
	return Location();
}

void Handle_request::get_handle() {}
void Handle_request::post_handle() {}
void Handle_request::delete_handle() {}

//////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////
std::vector<std::string> split_string(std::string str, std::string delimiter)
{
	size_t pos;
	std::string token;
	std::vector<std::string> final_vector;

	while ((pos = str.find(delimiter)) != std::string::npos)
	{
		token = str.substr(0, pos);
		final_vector.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	final_vector.push_back(str);
	return final_vector;
}
