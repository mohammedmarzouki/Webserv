#include "Handle_request_response.hpp"

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
Request::Request()
{
	_method = "NULL";
	_path = "NULL";
	_connection = "NULL";
	_content_length = 0;
	_transfer_encoding = "NULL";
	_header_status = RECEIVE;
	_status_code = 200;
}

void Request::set_method(std::string method) { this->_method = method; }
void Request::set_path(std::string path) { this->_path = path; }
void Request::set_connection(std::string connection) { this->_connection = connection; }
void Request::set_content_length(std::string content_length) { this->_content_length = atoi(content_length.c_str()); }
void Request::set_transfer_encoding(std::string transfer_encoding) { this->_transfer_encoding = transfer_encoding; }
void Request::set_temp_header(std::string temp_header) { this->_temp_header = temp_header; }
void Request::set_location(Location location) { this->_location = location; }
void Request::set_status_code(short status_code) { this->_status_code = status_code; }
void Request::set_header_status(short header_status) { this->_header_status = header_status; }
void Request::set_body_status(short body_status) { this->_body_status = body_status; }
void Request::set_read_bytes(size_t read_bytes) { this->_read_bytes = read_bytes; }
void Request::set_path_to_upload(std::string path_to_upload) { this->_path_to_upload = path_to_upload; }
std::string Request::get_method(void) const { return _method; }
std::string Request::get_path(void) const { return _path; }
std::string Request::get_connection(void) const { return _connection; }
size_t Request::get_content_length(void) const { return _content_length; }
std::string Request::get_transfer_encoding(void) const { return _transfer_encoding; }
std::string Request::get_temp_header(void) const { return _temp_header; }
Location Request::get_location(void) const { return _location; }
short Request::get_status_code(void) const { return _status_code; }
short Request::get_header_status(void) const { return _header_status; }
short Request::get_body_status(void) const { return _body_status; }
size_t Request::get_read_bytes(void) const { return _read_bytes; }
std::string Request::get_path_to_upload(void) const { return _path_to_upload; }

//////////////////////////////////////////////////
// Handle_request_response class
//////////////////////////////////////////////////
Handle_request_response::Handle_request_response() {}

int Handle_request_response::recv_request(int fd, Server &server)
{
	/// if server doesn't exist, add it to the map
	if (requests.find(fd) == requests.end())
		requests[fd] = Request();

	char temp[BUFFER_SIZE];
	size_t r = recv(fd, temp, BUFFER_SIZE - 1, 0);

	if (r < 1)
	{
		requests.erase(fd);
		return FAILED;
	}
	else
	{
		temp[r] = '\0';
		std::string received(temp, r);
		// recv header
		if (!requests[fd].get_header_status())
		{
			size_t header_end = received.find("\r\n\r\n");
			if (header_end == std::string::npos)
			{
				requests[fd].set_temp_header(requests[fd].get_temp_header() + received);
				return CHUNCKED;
			}
			else
			{
				received = requests[fd].get_temp_header() + received;
				requests[fd].set_header_status(READ);
			}
		}

		// parse header else parse body
		if (requests[fd].get_header_status() < PARSED)
		{
			int first_line_status = request_first_line(fd, received, server);
			if (first_line_status)
			{
				requests[fd].set_status_code(first_line_status);
				return DONE;
			}
			requests[fd].set_connection(find_value("Connection:", received));
			requests[fd].set_content_length(find_value("Content-Length:", received));
			if (requests[fd].get_content_length() > server.get_client_max_body_size())
			{
				requests[fd].set_status_code(PAYLOAD_TOO_LARGE);
				return DONE;
			}
			requests[fd].set_transfer_encoding(find_value("Transfer-Encoding:", received));
			if (requests[fd].get_transfer_encoding() != "NULL" && requests[fd].get_transfer_encoding() != "chunked")
			{
				requests[fd].set_status_code(NOT_IMPLEMENTED);
				return DONE;
			}
			fix_path(requests[fd]);
			requests[fd].set_header_status(PARSED);
		}
		// recv body only in case of POST request else ignore
		if (requests[fd].get_method() == "GET")
			return Handle_request_response::get_handle(fd, server);
		if (requests[fd].get_method() == "POST")
			return Handle_request_response::post_handle(fd, received, r);
		else
			return Handle_request_response::delete_handle(fd, server);
	}
}
int Handle_request_response::get_handle(int fd, Server &server)
{
	(void)fd;
	(void)server;
	return DONE;
}
int Handle_request_response::post_handle(int fd, std::string &received, int r)
{
	// to do:
	/// chunked parsing
	/// in case upload file name isn't given
	std::ofstream upload_file;
	if (requests[fd].get_header_status() == PARSED)
	{
		received = received.substr(received.find("\r\n\r\n") + 4);
		requests[fd].set_read_bytes(received.size());
		requests[fd].set_header_status(FULLY_PARSED);

		// if location accept POST and doesn't have directive is concidered an error
		if (requests[fd].get_location().get_upload() == "NULL")
		{
			requests[fd].set_status_code(501);
			return DONE;
		}

		// open file to upload to
		if (requests[fd].get_path() == requests[fd].get_location().get_upload())
			requests[fd].set_path_to_upload("mkdir -p " + requests[fd].get_path().substr(1));
		else
			requests[fd].set_path_to_upload("mkdir -p " + requests[fd].get_path().substr(1, requests[fd].get_path().find_last_of("/")));
		system(requests[fd].get_path_to_upload().c_str());
	}
	else
		requests[fd].set_read_bytes(requests[fd].get_read_bytes() + r);
	upload_file.open(requests[fd].get_path().substr(1).c_str(), std::ios::out | std::ios::app);

	// read from socket
	// writing what is left from first read (header read)
	upload_file << received;
	if (requests[fd].get_read_bytes() < requests[fd].get_content_length())
		return CHUNCKED;
	return DONE;
}
int Handle_request_response::delete_handle(int fd, Server &server)
{
	(void)fd;
	(void)server;
	return DONE;
}
int Handle_request_response::send_response(int fd)
{
	std::string header = header_maker(fd);
	std::cout << header;
	char buffer[BUFFER_SIZE];

	sprintf(buffer, "HTTP/1.1 200 OK\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Connection: keep_alive\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Content-Length: %u\r\n", 10);
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Content-Type: %s\r\n", "text/plain");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "tatatatata");
	send(fd, buffer, strlen(buffer), 0);

	return KEEP_ALIVE;
}

int Handle_request_response::request_first_line(int fd, std::string received, Server &server)
{
	size_t pos;
	size_t end_pos;

	if ((pos = received.find("GET")) == std::string::npos)
		if ((pos = received.find("POST")) == std::string::npos)
			if ((pos = received.find("DELETE")) == std::string::npos)
				return NOT_IMPLEMENTED;
	if ((end_pos = received.find("\r\n", pos)) == std::string::npos)
		return BAD_REQUEST;

	std::vector<std::string> splitted_first_line = split_string(received.substr(pos, end_pos - pos), " ");
	requests[fd].set_location(right_location(splitted_first_line[1], server));
	if (requests[fd].get_location().get_uri() == "NULL")
		return NOT_FOUND;

	if (is_method_allowed(requests[fd].get_location(), splitted_first_line[0]))
		requests[fd].set_method(splitted_first_line[0]);
	else
		return METHOD_NOT_ALLOWED;
	if (splitted_first_line[1].size() <= 1024)
		requests[fd].set_path(splitted_first_line[1]);
	else
		return REQUEST_URI_TOO_LONG;
	return 0;
}
std::string Handle_request_response::find_value(std::string key, std::string received)
{
	size_t pos = received.find(key);
	size_t end_pos = received.find("\r\n", pos);
	if (pos == std::string::npos || end_pos == std::string::npos)
		return "NULL";

	std::string whole_line;
	whole_line = received.substr(pos, end_pos - pos);
	std::vector<std::string> splitted_line = split_string(whole_line, " ");
	return splitted_line[1];
}
Location Handle_request_response::right_location(std::string path, Server &server)
{
	Location location;

	do
	{
		location = wanted_location(path, server);

		if (location.get_uri() == "NULL")
		{
			size_t pos = path.find_last_of("/");
			if (pos != std::string::npos)
				path = path.substr(0, pos);
		}
		else
			break;
	} while (path.size());

	// if location isn't found, use root location
	// if no root location is found an empty location is returned
	if (location.get_uri() == "NULL")
		location = wanted_location("/", server);

	return location;
}
Location Handle_request_response::wanted_location(std::string path, Server &server)
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
bool Handle_request_response::is_method_allowed(Location location, std::string method)
{
	std::vector<std::string> allow_methods = location.get_allow_methods();
	if (std::count(allow_methods.begin(), allow_methods.end(), method))
		return true;
	return false;
}
void Handle_request_response::fix_path(Request &request)
{
	Location location = request.get_location();
	std::string uri = request.get_method() == "POST" ? location.get_upload() : location.get_root();
	size_t route_size = location.get_uri().size();

	// uri.size() == 1 => "/"
	route_size = route_size == 1 ? 0 : route_size;
	request.set_path(uri + request.get_path().substr(route_size));
}
std::vector<std::string> Handle_request_response::split_string(std::string str, std::string delimiter)
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

std::string Handle_request_response::header_maker(short fd)
{
	std::string header;

	header += "HTTP/1.1 ";
	header += status_maker(requests[fd].get_status_code());
	header += "\r\n";
	if (requests[fd].get_method() == "GET")
	{
		header += "Content-Length: ";
		header += to_string(1000);
		header += "\r\n";
		header += "Content-Type: ";
		header += content_type_maker(fd);
		header += "\r\n";
	}
	header += "\r\n";
	return header;
}
std::string Handle_request_response::status_maker(short status_code)
{
	switch (status_code)
	{
	case CONTINUE:
		return ("100 Continue");
		break;
	case SWITCHING_PROTOCOLS:
		return ("101 Switching Protocols");
		break;
	case OK:
		return ("200 OK");
		break;
	case CREATED:
		return ("201 Created");
		break;
	case MOVED_PERMANENTLY:
		return ("301 Moved Permanently");
		break;
	case FOUND:
		return ("302 Found");
		break;
	case TEMPORARY_REDIRECT:
		return ("307 Temporary Redirect");
		break;
	case PERMANENT_REDIRECT:
		return ("308 Permanent Redirect");
		break;
	case BAD_REQUEST:
		return ("400 Bad Request");
		break;
	case FORBIDDEN:
		return ("403 Forbidden");
		break;
	case NOT_FOUND:
		return ("404 Not Found");
		break;
	case METHOD_NOT_ALLOWED:
		return ("405 Method Not Allowed");
		break;
	case PAYLOAD_TOO_LARGE:
		return ("413 Payload Too Large");
		break;
	case INTERNAL_SERVER_ERROR:
		return ("500 Internal Server Error");
		break;
	case NOT_IMPLEMENTED:
		return ("501 Not Implemented");
		break;
	case BAD_GATEWAY:
		return ("502 Bad Gateway");
		break;
	default:
		return to_string(status_code);
	}
}
std::string Handle_request_response::content_type_maker(int fd)
{
	(void)fd;
	return "application/json";
}
std::string Handle_request_response::to_string(int i)
{
	std::stringstream ss;
	std::string str;

	ss << i;
	ss >> str;
	return str;
}