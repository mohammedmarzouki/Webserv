#include "Handle_request_response.hpp"

//////////////////////////////////////////////////
// Request class
//////////////////////////////////////////////////
Request::Request()
{
	_method = "NULL";
	_path = "NULL";
	_connection = "keep-alive";
	_content_length = 0;
	_content_type = "";
	_transfer_encoding = "NULL";
	_temp_header = "";
	_status_code = 200;
	_header_status = RECEIVE;
	_read_bytes = 0;
	_path_to_upload = "";
	_chunked_bytes = -1;
	_chunked_temp = "";
}

void Request::set_method(std::string method) { this->_method = method; }
void Request::set_path(std::string path) { this->_path = path; }
void Request::set_host(std::string host) { this->_host = host; }
void Request::set_port(std::string port) { this->_port = port; }
void Request::set_connection(std::string connection) { this->_connection = connection; }
void Request::set_content_length(std::string content_length) { this->_content_length = atoi(content_length.c_str()); }
void Request::set_content_type(std::string content_type) { this->_content_type = content_type; }
void Request::set_transfer_encoding(std::string transfer_encoding) { this->_transfer_encoding = transfer_encoding; }
void Request::set_temp_header(std::string temp_header) { this->_temp_header = temp_header; }
void Request::set_location(Location location) { this->_location = location; }
void Request::set_status_code(short status_code) { this->_status_code = status_code; }
void Request::set_header_status(short header_status) { this->_header_status = header_status; }
void Request::set_read_bytes(size_t read_bytes) { this->_read_bytes = read_bytes; }
void Request::set_path_to_upload(std::string path_to_upload) { this->_path_to_upload = path_to_upload; }
void Request::set_chunked_bytes(long chunked_bytes) { this->_chunked_bytes = chunked_bytes; }
void Request::set_chunked_temp(std::string chunked_temp) { this->_chunked_temp = chunked_temp; }
std::string Request::get_method() const { return _method; }
std::string Request::get_path() const { return _path; }
std::string Request::get_host() const { return _host; }
std::string Request::get_port() const { return _port; }
std::string Request::get_connection() const { return _connection; }
size_t Request::get_content_length() const { return _content_length; }
std::string Request::get_content_type() const { return _content_type; }
std::string Request::get_transfer_encoding() const { return _transfer_encoding; }
std::string Request::get_temp_header() const { return _temp_header; }
Location Request::get_location() const { return _location; }
short Request::get_status_code() const { return _status_code; }
short Request::get_header_status() const { return _header_status; }
size_t Request::get_read_bytes() const { return _read_bytes; }
std::string Request::get_path_to_upload() const { return _path_to_upload; }
long Request::get_chunked_bytes() const { return _chunked_bytes; }
std::string Request::get_chunked_temp() const { return _chunked_temp; }

void Request::clear_request()
{
	_method = "NULL";
	_path = "NULL";
	_connection = "NULL";
	_content_length = 0;
	_content_type = "";
	_transfer_encoding = "NULL";
	_temp_header = "";
	_status_code = 200;
	_header_status = RECEIVE;
	_read_bytes = 0;
	_path_to_upload = "";
	_chunked_bytes = -1;
	_chunked_temp = "";
}

//////////////////////////////////////////////////
// Response class
//////////////////////////////////////////////////
Response::Response()
{
	_header = "";
	_header_sent = HEADER_NOT_SENT;
	_cgi = "";
	_cgi_path = "";
	_autoindex = false;
	_content_length = 0;
	_sent_sofar = 0;
}

void Response::set_header(std::string header) { this->_header = header; }
void Response::set_header_sent(bool header_sent) { this->_header_sent = header_sent; }
void Response::set_cgi(std::string cgi) { this->_cgi = cgi; }
void Response::set_cgi_path(std::string cgi_path) { this->_cgi_path = cgi_path; }
void Response::set_autoindex(bool autoindex) { this->_autoindex = autoindex; }
void Response::set_content_length(unsigned long content_length) { this->_content_length = content_length; }
void Response::set_sent_sofar(unsigned long sent_sofar) { this->_sent_sofar = sent_sofar; }
std::string Response::get_header() const { return _header; }
bool Response::get_header_sent() const { return _header_sent; }
std::string Response::get_cgi() const { return _cgi; }
std::string Response::get_cgi_path() const { return _cgi_path; }
bool Response::get_autoindex() const { return _autoindex; }
unsigned long Response::get_content_length() const { return _content_length; }
unsigned long Response::get_sent_sofar() const { return _sent_sofar; }

void Response::clear_response()
{
	_header = "";
	_header_sent = HEADER_NOT_SENT;
	_cgi = "";
	_cgi_path = "";
	_autoindex = false;
	_content_length = 0;
	_sent_sofar = 0;
}

//////////////////////////////////////////////////
// Handle_request_response class
//////////////////////////////////////////////////

//////////////////////////////////////////////////
// Request
//////////////////////////////////////////////////
Handle_request_response::Handle_request_response() {}

int Handle_request_response::recv_request(int fd, Server &server)
{
	/// if server doesn't exist, add it to the map
	if (requests.find(fd) == requests.end())
	{
		requests[fd].first = Request();
		requests[fd].second = Response();
	}

	char temp[BUFFER_SIZE];
	long r = recv(fd, temp, BUFFER_SIZE - 1, 0);

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
				requests[fd].first.set_header_status(READ);
			}
		}

		// parse header
		if (requests[fd].first.get_header_status() < PARSED)
		{
			int first_line_status = request_first_line(fd, received, server);
			if (first_line_status)
			{
				requests[fd].first.set_status_code(first_line_status);
				return DONE;
			}
			std::string host_port = find_value("Host:", received);
			requests[fd].first.set_host(split_string(host_port, ":")[0]);
			requests[fd].first.set_port(split_string(host_port, ":")[1]);
			requests[fd].first.set_connection(find_value("Connection:", received));
			requests[fd].first.set_transfer_encoding(find_value("Transfer-Encoding:", received));
			if (requests[fd].first.get_transfer_encoding() != "NULL" && requests[fd].first.get_transfer_encoding() != "chunked")
			{
				requests[fd].first.set_status_code(NOT_IMPLEMENTED);
				return DONE;
			}
			requests[fd].first.set_content_length(find_value("Content-Length:", received));
			if (requests[fd].first.get_content_length() > server.get_client_max_body_size())
			{
				requests[fd].first.set_status_code(PAYLOAD_TOO_LARGE);
				return DONE;
			}
			requests[fd].first.set_content_type(find_value("Content-Type:", received));
			fix_path(requests[fd].first);
			requests[fd].first.set_header_status(PARSED);
		}

		if (requests[fd].first.get_method() == "GET")
			return Handle_request_response::get_handle(fd);
		if (requests[fd].first.get_method() == "POST")
			return Handle_request_response::post_handle(fd, received, r);
		else
			return Handle_request_response::delete_handle(fd);
	}
}
int Handle_request_response::get_handle(int fd)
{
	struct stat info;
	std::string path = requests[fd].first.get_path();
	std::string ext = ext_from_path(path);
	if (stat(path.c_str(), &info) != 0)
	{
		// request resource not found
		requests[fd].first.set_status_code(NOT_FOUND);
		return DONE;
	}
	else if (S_ISREG(info.st_mode))
	{
		// is registery
		std::vector<std::string> cgi = requests[fd].first.get_location().get_cgi();
		std::vector<std::string>::iterator it;

		if ((it = find(cgi.begin(), cgi.end(), ext)) != cgi.end())
		{
			requests[fd].second.set_cgi(*it++);
			requests[fd].second.set_cgi_path(*it);
			// running CGI code here
		}
		else
			requests[fd].second.set_content_length(info.st_size);
		return DONE;
	}
	else // if (S_ISDIR(info.st_mode))
	{
		// is directory
		std::vector<std::string> index = requests[fd].first.get_location().get_index();
		if (index.size())
		{
			// if there is an index file
			std::vector<std::string>::iterator it = index.begin();
			std::string index_path;
			struct stat index_info;
			while (it != index.end())
			{
				index_path = path + *it;
				if (stat(index_path.c_str(), &index_info) == 0 && S_ISREG(index_info.st_mode))
				{
					requests[fd].first.set_path(index_path);
					break;
				}
				it++;
			}
			if (requests[fd].first.get_path() != index_path)
			{
				// if no index is found check autoindex
				if (requests[fd].first.get_location().get_autoindex() == "on")
					requests[fd].second.set_autoindex(true);
				else
					requests[fd].first.set_status_code(FORBIDDEN);
			}
			else
				get_handle(fd);
		}
		else
		{
			// if autoindex is on
			if (requests[fd].first.get_location().get_autoindex() == "on")
				requests[fd].second.set_autoindex(true);
			else
				requests[fd].first.set_status_code(FORBIDDEN);
		}
		return DONE;
	}
}
int Handle_request_response::post_handle(int fd, std::string &received, int r)
{
	if (requests[fd].first.get_header_status() == PARSED)
	{
		// if location accept POST and doesn't have upload directive is concidered an error
		if (requests[fd].first.get_location().get_upload() == "NULL")
		{
			requests[fd].first.set_status_code(NOT_IMPLEMENTED);
			return DONE;
		}
		received = received.substr(received.find("\r\n\r\n") + 4);
		requests[fd].first.set_read_bytes(received.size());
		requests[fd].first.set_header_status(FULLY_PARSED);

		// create folder to upload to
		requests[fd].first.set_path_to_upload("mkdir -p " + requests[fd].first.get_location().get_upload().substr(1));
		system(requests[fd].first.get_path_to_upload().c_str());
		requests[fd].first.set_path_to_upload(requests[fd].first.get_location().get_upload().substr(1) + "/" + generate_random_name() + extension_maker(requests[fd].first.get_content_type()));
	}
	else if (requests[fd].first.get_transfer_encoding() != "chunked") // counting not needed in case of chunked
		requests[fd].first.set_read_bytes(requests[fd].first.get_read_bytes() + r);

	if (requests[fd].first.get_transfer_encoding() == "chunked")
	{
		size_t endline_pos;
		requests[fd].first.set_chunked_temp(requests[fd].first.get_chunked_temp() + received);
		endline_pos = requests[fd].first.get_chunked_temp().find("\r\n");

		while (endline_pos != std::string::npos)
		{
			if (requests[fd].first.get_chunked_bytes() == -1)
			{
				requests[fd].first.set_chunked_bytes(hex_to_dec(requests[fd].first.get_chunked_temp().substr(0, endline_pos)));
				if (!requests[fd].first.get_chunked_bytes())
				{
					requests[fd].first.set_status_code(NO_CONTENT);
					return DONE;
				}
				requests[fd].first.set_chunked_temp(requests[fd].first.get_chunked_temp().substr(endline_pos + 2));
				endline_pos = requests[fd].first.get_chunked_temp().find("\r\n");
			}
			else
			{
				size_t chunked_bytes = requests[fd].first.get_chunked_bytes();
				if (requests[fd].first.get_chunked_temp().size() >= chunked_bytes)
				{
					std::ofstream upload_file(requests[fd].first.get_path_to_upload(), std::ios::out | std::ios::app);
					upload_file << requests[fd].first.get_chunked_temp().substr(0, chunked_bytes);
					upload_file.close();
					requests[fd].first.set_chunked_temp(requests[fd].first.get_chunked_temp().substr(chunked_bytes + 2));
					requests[fd].first.set_chunked_bytes(-1);
					endline_pos = requests[fd].first.get_chunked_temp().find("\r\n");
				}
				else
					break;
			}
		}
		return CHUNKED;
	}
	else
	{
		std::ofstream upload_file(requests[fd].first.get_path_to_upload(), std::ios::out | std::ios::app);
		upload_file << received;
		upload_file.close();

		if (requests[fd].first.get_read_bytes() < requests[fd].first.get_content_length())
			return CHUNCKED;
	}
	requests[fd].first.set_status_code(NO_CONTENT);
	return DONE;
}
int Handle_request_response::delete_handle(int fd)
{
	struct stat info;
	std::string path = requests[fd].first.get_path();

	if (stat(path.c_str(), &info) != 0)
		requests[fd].first.set_status_code(NOT_FOUND);
	else
	{
		std::string path_to_delete = "rm -rf " + path;
		int i = system(path_to_delete.c_str());
		if (!i)
			requests[fd].first.set_status_code(NO_CONTENT);
		else
			requests[fd].first.set_status_code(INTERNAL_SERVER_ERROR);
	}
	return DONE;
}

//////////////////////////////////////////////////
// Request tools
//////////////////////////////////////////////////
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
	requests[fd].first.set_location(right_location(splitted_first_line[1], server));
	if (requests[fd].first.get_location().get_uri() == "NULL")
		return NOT_FOUND;

	if (is_method_allowed(requests[fd].first.get_location(), splitted_first_line[0]))
		requests[fd].first.set_method(splitted_first_line[0]);
	else
		return METHOD_NOT_ALLOWED;
	if (splitted_first_line[1].size() <= 1024)
		requests[fd].first.set_path(splitted_first_line[1]);
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
	std::string uri = request.get_method() == "POST" ? location.get_upload().substr(1) : location.get_root().substr(1);
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
std::string Handle_request_response::generate_random_name()
{
	std::string random_name;

	srand(time(0));
	random_name = "file_" + int_to_str(rand());
	return random_name;
}

//////////////////////////////////////////////////
// Response
//////////////////////////////////////////////////
int Handle_request_response::send_response(int fd, Server &server)
{
	if (requests[fd].first.get_status_code() != 200 && requests[fd].first.get_status_code() != 204)
	{
		std::string error_page;
		std::vector<std::string> defined_error_pages = server.get_error_page();
		std::string error_html = defined_error_page_found(defined_error_pages, requests[fd].first.get_status_code());
		if (!error_html.size())
			error_html = error_page_maker(requests[fd].first.get_status_code());

		error_page += "HTTP/1.1 " + status_code_maker(requests[fd].first.get_status_code());
		error_page += "Content-Type: text/html\r\n";
		error_page += "Content-Length: " + int_to_str(error_html.size()) + "\r\n\r\n";
		error_page += error_html;
		send_string(fd, error_page);
		requests[fd].first.clear_request();
		requests[fd].second.clear_response();
		return KEEP_ALIVE;
	}
	std::string autoindex_file;
	if (requests[fd].second.get_autoindex() == true)
	{
		autoindex_file = autoindex_maker(fd);
		requests[fd].second.set_content_length(autoindex_file.size());
	}
	// send header
	if (!requests[fd].second.get_header_sent())
	{
		if (requests[fd].second.get_header().size() == 0)
			requests[fd].second.set_header(header_maker(fd));
		long s = send(fd, requests[fd].second.get_header().c_str(), requests[fd].second.get_header().size(), 0);
		if (s < (long)requests[fd].second.get_header().size())
		{
			requests[fd].second.set_header(requests[fd].second.get_header().substr(s));
			return KEEP_ALIVE;
		}
		else
			requests[fd].second.set_header_sent(HEADER_SENT);
	}
	// send body in case of GET request
	if (requests[fd].first.get_method() == "GET")
	{
		// send body code
		if (requests[fd].second.get_autoindex() == true)
			send_string(fd, autoindex_file);
		else
		{
			std::ifstream requested_file(requests[fd].first.get_path());
			char buffer[BUFFER_SIZE] = {[0 ... BUFFER_SIZE - 1] = 0};
			long read;

			if (requests[fd].second.get_content_length() - requests[fd].second.get_sent_sofar())
			{
				requested_file.seekg(requests[fd].second.get_sent_sofar());
				requested_file.read(buffer, BUFFER_SIZE - 1);
				read = requested_file.gcount();
				buffer[read] = '\0';

				long sent = send(fd, buffer, read, 0);
				if (sent < 0)
				{
					requests[fd].first.clear_request();
					requests[fd].second.clear_response();
					return KILL_CONNECTION;
				}
				requests[fd].second.set_sent_sofar(requests[fd].second.get_sent_sofar() + sent);
				requested_file.close();
				return CHUNCKED;
			}
		}
		requests[fd].first.clear_request();
		requests[fd].second.clear_response();
		return KEEP_ALIVE;
	}
	else
	{
		requests[fd].first.clear_request();
		requests[fd].second.clear_response();
		return KEEP_ALIVE;
	}
}

//////////////////////////////////////////////////
// Response tools
//////////////////////////////////////////////////
std::string Handle_request_response::header_maker(short fd)
{
	std::string header;

	header = "HTTP/1.1 " + status_code_maker(requests[fd].first.get_status_code());
	header += "Connection: ";
	header += requests[fd].first.get_connection();
	header += "\r\n";
	if (requests[fd].first.get_method() == "GET")
	{
		header += "Content-Length: ";
		header += int_to_str(requests[fd].second.get_content_length());
		header += "\r\n";
		if (requests[fd].second.get_autoindex())
			header += "Content-Type: text/html\r\n";
		else
			header += content_type_maker(ext_from_path(requests[fd].first.get_path()));
	}
	header += "\r\n";
	return header;
}
std::string Handle_request_response::status_code_maker(short status_code)
{
	switch (status_code)
	{
	case OK:
		return "200 OK\r\n";
		break;
	case CREATED:
		return "201 Created\r\n";
		break;
	case NO_CONTENT:
		return "204 No Content\r\n";
		break;
	case MOVED_PERMANENTLY:
		return "301 Moved Permanently\r\n";
		break;
	case FOUND:
		return "302 Found\r\n";
		break;
	case TEMPORARY_REDIRECT:
		return "307 Temporary Redirect\r\n";
		break;
	case PERMANENT_REDIRECT:
		return "308 Permanent Redirect\r\n";
		break;
	case BAD_REQUEST:
		return "400 Bad Request\r\n";
		break;
	case FORBIDDEN:
		return "403 Forbidden\r\n";
		break;
	case NOT_FOUND:
		return "404 Not Found\r\n";
		break;
	case METHOD_NOT_ALLOWED:
		return "405 Method Not Allowed\r\n";
		break;
	case PAYLOAD_TOO_LARGE:
		return "413 Payload Too Large\r\n";
		break;
	case REQUEST_URI_TOO_LONG:
		return "414 URI Too Long\r\n";
		break;
	case INTERNAL_SERVER_ERROR:
		return "500 Internal Server Error\r\n";
		break;
	case NOT_IMPLEMENTED:
		return "501 Not Implemented\r\n";
		break;
	default:
		return int_to_str(status_code);
	}
}
std::string Handle_request_response::content_type_maker(std::string ext)
{
	std::string base("Content-Type: ");

	if (ext == "html" || ext == "htm")
		return base + "text/html\r\n";
	else if (ext == "css")
		return base + "text/css\r\n";
	else if (ext == "xml")
		return base + "text/xml\r\n";
	else if (ext == "gif")
		return base + "image/gif\r\n";
	else if (ext == "jpeg" || ext == "jpg")
		return base + "image/jpeg\r\n";
	else if (ext == "js")
		return base + "application/javascript\r\n";
	else if (ext == "txt")
		return base + "text/plain\r\n";
	else if (ext == "png")
		return base + "image/png\r\n";
	else if (ext == "svg" || ext == "svgz")
		return base + "image/svg+xml\r\n";
	else if (ext == "ico")
		return base + "image/x-icon\r\n";
	else if (ext == "json")
		return base + "application/json\r\n";
	else if (ext == "pdf")
		return base + "application/pdf\r\n";
	else if (ext == "csv")
		return base + "text/csv\r\n";
	else if (ext == "ppt")
		return base + "application/vnd.ms-powerpoint\r\n";
	else if (ext == "zip")
		return base + "application/zip\r\n";
	else if (ext == "mp3")
		return base + "audio/mpeg\r\n";
	else if (ext == "mp4")
		return base + "video/mp4\r\n";
	return base + "application/octet-stream\r\n";
}
std::string Handle_request_response::extension_maker(std::string type)
{
	if (type == "text/html")
		return ".html";
	else if (type == "text/css")
		return ".css";
	else if (type == "text/xml")
		return ".xml";
	else if (type == "image/gif")
		return ".gif";
	else if (type == "image/jpeg")
		return ".jpeg";
	else if (type == "application/javascript")
		return ".js";
	else if (type == "text/plain")
		return ".txt";
	else if (type == "image/png")
		return ".png";
	else if (type == "image/svg+xml")
		return ".svg";
	else if (type == "image/x-icon")
		return ".ico";
	else if (type == "application/json")
		return ".json";
	else if (type == "application/pdf")
		return ".pdf";
	else if (type == "text/csv")
		return ".csv";
	else if (type == "application/vnd.ms-powerpoint")
		return ".ppt";
	else if (type == "application/zip")
		return ".zip";
	else if (type == "audio/mpeg")
		return ".mp3";
	else if (type == "video/mp4")
		return ".mp4";
	return "";
}
std::string Handle_request_response::ext_from_path(std::string path)
{
	size_t dot_pos = path.find_last_of(".");
	if (dot_pos != std::string::npos)
		return path.substr(++dot_pos);
	return "";
}
std::string Handle_request_response::autoindex_maker(int fd)
{
	DIR *dr;
	struct dirent *en;

	std::string autoindex_file;
	autoindex_file = "\
	<!DOCTYPE html>\n\
	<html lang=\"en\">\n\
		<head>\n\
			<meta charset=\"UTF-8\">\n\
			<title>Index</title>\n\
		</head>\n\
		<body>\n\
		<h1>Index</h1>\n\
		<hr>\n<ul>\n";

	dr = opendir(requests[fd].first.get_path().c_str());
	while ((en = readdir(dr)) != NULL)
	{
		if (!strcmp(en->d_name, "."))
			continue;
		else if (!strcmp(en->d_name, ".."))
			autoindex_file += "\t\t<li><a href=\"..\">..</a></li>\n";
		else
		{
			autoindex_file += "\t\t<li><a href=\"http://";
			autoindex_file += requests[fd].first.get_host() + ":" + requests[fd].first.get_port();
			autoindex_file += requests[fd].first.get_path().substr(requests[fd].first.get_path().find("/"));
			autoindex_file += requests[fd].first.get_path().back() != '/' ? "/" : "";
			autoindex_file += en->d_name;
			autoindex_file += "\">";
			autoindex_file += en->d_name;
			autoindex_file += "</a></li>\n";
		}
	}
	autoindex_file += "\t</ul>\n</body>\n</html>";
	closedir(dr);
	return autoindex_file;
}
std::string Handle_request_response::error_page_maker(short status_code)
{
	std::string error_page;

	error_page = "\
	<!DOCTYPE html>\n\
	<html lang=\"en\">\n\
		<head>\n\
			<meta charset=\"UTF-8\">\n\
			<title>Error Page</title>\n\
		</head>\n\
		<body>\n\
		<h1>\n";
	error_page += status_code_maker(status_code);
	error_page += "</h1>\n</body>\n</html>\n";
	return error_page;
}
void Handle_request_response::send_string(int fd, std::string to_send)
{
	size_t len = to_send.size();
	size_t total_send = 0;

	do
	{
		size_t sent = send(fd, to_send.c_str(), to_send.size(), 0);
		to_send = to_send.substr(sent);
		total_send += sent;
	} while (total_send < len);
}
std::string Handle_request_response::defined_error_page_found(std::vector<std::string> &defined_error_pages, short status_code)
{
	std::vector<std::string>::iterator it;
	it = find(defined_error_pages.begin(), defined_error_pages.end(), int_to_str(status_code));
	if (it != defined_error_pages.end())
	{
		std::string error_page = *(++it);
		std::ifstream file(error_page.substr(1));
		if (file.good())
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			return buffer.str();
		}
	}
	return "";
}
std::string Handle_request_response::int_to_str(int i)
{
	std::stringstream ss;
	std::string str;

	ss << i;
	ss >> str;
	return str;
}
int Handle_request_response::str_to_int(std::string str)
{
	std::stringstream ss;
	int i;

	ss << str;
	ss >> i;
	return i;
}
int Handle_request_response::hex_to_dec(std::string hex_val)
{
	int base = 1;
	int dec_val = 0;

	for (int i = hex_val.size() - 1; i >= 0; i--)
	{
		if (hex_val[i] >= '0' && hex_val[i] <= '9')
			dec_val += (int(hex_val[i]) - 48) * base;
		else if (hex_val[i] >= 'a' && hex_val[i] <= 'f')
			dec_val += (int(hex_val[i]) - 87) * base;
		base = base * 16;
	}
	return dec_val;
}
