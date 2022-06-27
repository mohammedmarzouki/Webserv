#include "Handle_request.hpp"

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
void Request::set_temp_body(std::string temp_body) { this->_temp_body = temp_body; }
void Request::set_location(Location location) { this->_location = location; }
void Request::set_status_code(short status_code) { this->_status_code = status_code; }
void Request::set_header_status(short header_status) { this->_header_status = header_status; }
void Request::set_body_status(short body_status) { this->_body_status = body_status; }
std::string Request::get_method(void) const { return _method; }
std::string Request::get_path(void) const { return _path; }
std::string Request::get_connection(void) const { return _connection; }
int Request::get_content_length(void) const { return _content_length; }
std::string Request::get_transfer_encoding(void) const { return _transfer_encoding; }
std::string Request::get_temp_header(void) const { return _temp_header; }
std::string Request::get_temp_body(void) const { return _temp_body; }
Location Request::get_location(void) const { return _location; }
short Request::get_status_code(void) const { return _status_code; }
short Request::get_header_status(void) const { return _header_status; }
short Request::get_body_status(void) const { return _body_status; }

//////////////////////////////////////////////////
// Response class
//////////////////////////////////////////////////
Response::Response()
{
	_status_line = "HTTP/1.1";
	_connection = "NULL";
	_content_length = "NULL";
	_content_type = "NULL";
}

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
	char temp[BUFFER_SIZE];

	/// if server doesn't exist, add it to the map
	if (requests.find(fd) == requests.end())
	{
		requests[fd].first = Request();
		requests[fd].second = Response();
	}

	int r = recv(fd, temp, BUFFER_SIZE, 0);
	std::string received(temp);
	if (r < 1)
	{
		requests.erase(fd);
		return FAILED;
	}
	else
	{
		// recv header
		if (!requests[fd].first.get_header_status())
		{
			size_t header_end = received.find("\r\n\r\n");
			if (header_end == std::string::npos)
			{
				requests[fd].first.set_temp_header(requests[fd].first.get_temp_header() + received);
				return CHUNCKED;
			}
			else
			{
				received = requests[fd].first.get_temp_header() + received;
				requests[fd].first.set_temp_body(received.substr(header_end + 4));
				requests[fd].first.set_header_status(READ);
			}
		}

		// parse header else parse body
		if (requests[fd].first.get_header_status() < PARSED)
		{
			int first_line_status = request_first_line(fd, received, server);
			if (first_line_status)
			{
				requests[fd].first.set_status_code(first_line_status);
				return DONE;
			}
			requests[fd].first.set_connection(find_value("Connection:", received));
			requests[fd].first.set_content_length(find_value("Content-Length:", received));
			if (requests[fd].first.get_content_length() > server.get_client_max_body_size())
			{
				requests[fd].first.set_status_code(413);
				return DONE;
			}
			requests[fd].first.set_transfer_encoding(find_value("Transfer-Encoding:", received));
			if (requests[fd].first.get_transfer_encoding() != "NULL" && requests[fd].first.get_transfer_encoding() != "chunked")
			{
				requests[fd].first.set_status_code(501);
				return DONE;
			}
			fix_path(requests[fd].first);
			requests[fd].first.set_header_status(PARSED);
		}
		// recv body only in case of POST request else ignore
		if (requests[fd].first.get_method() == "POST")
		{
			// - right location
			// - is_method_allowed
			// - fix_path
			// are already checked on request_first_line()

			// if location accept POST and doesn't have directive is concidered an error
			if (requests[fd].first.get_location().get_upload() == "NULL")
			{
				requests[fd].first.set_status_code(501);
				return DONE;
			}

			// open file to upload to
			std::string path_to_upload;
			std::ofstream outfile;

			if (requests[fd].first.get_path() == requests[fd].first.get_location().get_upload())
				path_to_upload = "mkdir -p " + requests[fd].first.get_path().substr(1);
			else
				path_to_upload = "mkdir -p " + requests[fd].first.get_path().substr(1, requests[fd].first.get_path().find_last_of("/"));
			system(path_to_upload.c_str());
			outfile.open(requests[fd].first.get_path().substr(1).c_str());
			
		}
	}
	return treat_request(fd, server);
}
int Handle_request::treat_request(int fd, Server &server)
{
	if (requests[fd].first.get_method() == "GET")
		return Handle_request::get_handle(fd, server);
	else if (requests[fd].first.get_method() == "POST")
		return Handle_request::post_handle(fd, server);
	else
		return Handle_request::delete_handle(fd, server);
}
int Handle_request::send_response(int fd)
{
	char buffer[BUFFER_SIZE];

	sprintf(buffer, "HTTP/1.1 200 OK\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Connection: close\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Content-Length: %u\r\n", 10);
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "Content-Type: %s\r\n", "text/plain");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "\r\n");
	send(fd, buffer, strlen(buffer), 0);

	sprintf(buffer, "tatatatata");
	send(fd, buffer, strlen(buffer), 0);

	return KILL_CONNECTION;
}

int Handle_request::get_handle(int fd, Server &server)
{
	(void)fd;
	(void)server;
	return DONE;
}
int Handle_request::post_handle(int fd, Server &server)
{
	(void)fd;
	(void)server;
	return DONE;
}
int Handle_request::delete_handle(int fd, Server &server)
{
	(void)fd;
	(void)server;
	return DONE;
}

int Handle_request::request_first_line(int fd, std::string received, Server &server)
{
	size_t pos;
	size_t end_pos;

	if ((pos = received.find("GET")) == std::string::npos)
		if ((pos = received.find("POST")) == std::string::npos)
			if ((pos = received.find("DELETE")) == std::string::npos)
				return 501;
	if ((end_pos = received.find("\r\n", pos)) == std::string::npos)
		return 400;

	std::vector<std::string> splitted_first_line = split_string(received.substr(pos, end_pos - pos), " ");
	requests[fd].first.set_location(right_location(splitted_first_line[1], server));
	if (requests[fd].first.get_location().get_uri() == "NULL")
		return 404;

	if (is_method_allowed(requests[fd].first.get_location(), splitted_first_line[0]))
		requests[fd].first.set_method(splitted_first_line[0]);
	else
		return 405;
	if (splitted_first_line[1].size() <= 1024)
		requests[fd].first.set_path(splitted_first_line[1]);
	else
		return 414;
	return 0;
}
std::string Handle_request::find_value(std::string key, std::string received)
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
Location Handle_request::right_location(std::string path, Server &server)
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
bool Handle_request::is_method_allowed(Location location, std::string method)
{
	std::vector<std::string> allow_methods = location.get_allow_methods();
	if (std::count(allow_methods.begin(), allow_methods.end(), method))
		return true;
	return false;
}
void Handle_request::fix_path(Request &request)
{
	Location location = request.get_location();
	std::string uri = request.get_method() == "POST" ? location.get_upload() : location.get_root();
	size_t route_size = location.get_uri().size();

	// uri.size() == 1 => "/"
	route_size = route_size == 1 ? 0 : route_size;
	request.set_path(uri + request.get_path().substr(route_size));
}
std::vector<std::string> Handle_request::split_string(std::string str, std::string delimiter)
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
