#include "Utils.hpp"

//////////////////////////////////////////////////
// Token class
//////////////////////////////////////////////////
webserv::Token::Token() {}
webserv::Token::Token(std::string type, std::string value) : _type(type), _value(value) {}
webserv::Token::~Token() {}

void webserv::Token::set_type(std::string type) { _type = type; }
void webserv::Token::set_value(std::string value) { _value = value; }

std::string webserv::Token::get_type() { return _type; }
std::string webserv::Token::get_value() { return _value; }

//////////////////////////////////////////////////
// Regex class
//////////////////////////////////////////////////
webserv::Regex::Regex() {}
webserv::Regex::~Regex() {}

std::string webserv::Regex::match(std::string &regex, std::string &string)
{
	if (regex == "whitespace")
		return match_whitespace(string);
	else if (regex == "comment")
		return match_comment(string);

	else if (regex == "{")
		return match_start_block(string);
	else if (regex == "}")
		return match_end_block(string);
	else if (regex == ";")
		return match_semicolon(string);

	else if (regex == "server")
		return match_server(string);
	else if (regex == "host")
		return match_host(string);
	else if (regex == "port")
		return match_port(string);
	else if (regex == "server_name")
		return match_server_name(string);
	else if (regex == "error_page")
		return match_error_page(string);
	else if (regex == "client_max_body_size")
		return match_client_max_body_size(string);

	else if (regex == "location")
		return match_location(string);
	else if (regex == "uri")
		return match_uri(string);
	else if (regex == "root")
		return match_root(string);
	else if (regex == "index")
		return match_index(string);
	else if (regex == "allow_methods")
		return match_allow_methods(string);
	else if (regex == "redirect")
		return match_return(string);
	else if (regex == "autoindex")
		return match_autoindex(string);
	else if (regex == "cgi_pass")
		return match_cgi_pass(string);

	else if (regex == "ipv4")
		return match_ipv4(string);
	else if (regex == "number")
		return match_number(string);
	else if (regex == "string")
		return match_string(string);
	else
		return "null";
}

std::string webserv::Regex::match_whitespace(std::string &string)
{
	if (isspace(string[0]))
	{
		std::string::size_type pos = string.find_first_not_of(" \t\n");
		if (pos != std::string::npos)
			return string.substr(0, pos);
		else
			return string;
	}
	return "null";
}
std::string webserv::Regex::match_comment(std::string &string)
{
	if (string[0] == '#')
	{
		std::string::size_type pos = string.find("\n");
		if (pos != std::string::npos)
			return string.substr(0, pos);
		else
			return string;
	}
	return "null";
}

std::string webserv::Regex::match_start_block(std::string &string)
{
	return string[0] == '{' ? string.substr(0, 1) : "null";
}
std::string webserv::Regex::match_end_block(std::string &string)
{
	return string[0] == '}' ? string.substr(0, 1) : "null";
}
std::string webserv::Regex::match_semicolon(std::string &string)
{
	return string[0] == ';' ? string.substr(0, 1) : "null";
}

std::string webserv::Regex::match_server(std::string &string)
{
	std::string server = string.substr(0, 6);
	return server == "server" ? server : "null";
}
std::string webserv::Regex::match_host(std::string &string)
{
	std::string host = string.substr(0, 4);
	return host == "host" ? host : "null";
}
std::string webserv::Regex::match_port(std::string &string)
{
	std::string port = string.substr(0, 4);
	return port == "port" ? port : "null";
}
std::string webserv::Regex::match_server_name(std::string &string)
{
	std::string server_name = string.substr(0, 11);
	return server_name == "server_name" ? server_name : "null";
}
std::string webserv::Regex::match_error_page(std::string &string)
{
	std::string error_page = string.substr(0, 10);
	return error_page == "error_page" ? error_page : "null";
}
std::string webserv::Regex::match_client_max_body_size(std::string &string)
{
	std::string client_max_body_size = string.substr(0, 20);
	return client_max_body_size == "client_max_body_size" ? client_max_body_size : "null";
}

std::string webserv::Regex::match_location(std::string &string)
{
	std::string location = string.substr(0, 8);
	return location == "location" ? location : "null";
}
std::string webserv::Regex::match_uri(std::string &string)
{
	if (string[0] == '/')
	{
		std::string::size_type pos = string.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ./_");
		if (pos != std::string::npos)
			return string.substr(0, pos);
		else
			return string;
	}
	return "null";
}
std::string webserv::Regex::match_root(std::string &string)
{
	std::string root = string.substr(0, 4);
	return root == "root" ? root : "null";
}
std::string webserv::Regex::match_index(std::string &string)
{
	std::string index = string.substr(0, 5);
	return (index == "index" && isspace(string[5])) ? index : "null";
}
std::string webserv::Regex::match_allow_methods(std::string &string)
{
	std::string allow_methods = string.substr(0, 13);
	return allow_methods == "allow_methods" ? allow_methods : "null";
}
std::string webserv::Regex::match_return(std::string &string)
{
	std::string redirect = string.substr(0, 6);
	return redirect == "return" ? redirect : "null";
}
std::string webserv::Regex::match_autoindex(std::string &string)
{
	std::string autoindex = string.substr(0, 9);
	return autoindex == "autoindex" ? autoindex : "null";
}
std::string webserv::Regex::match_cgi_pass(std::string &string)
{
	std::string cgi_pass = string.substr(0, 8);
	return cgi_pass == "cgi_pass" ? cgi_pass : "null";
}

std::string webserv::Regex::match_ipv4(std::string &string)
{
	if (isdigit(string[0]))
	{
		std::string::size_type pos = string.find_first_not_of("0123456789.");
		std::string address = string.substr(0, pos);

		return address.find(".") != std::string::npos ? address : "null"; // if there is a dot, it is an ipv4 address
	}
	return "null";
}
std::string webserv::Regex::match_number(std::string &string)
{
	if (isdigit(string[0]))
	{
		std::string::size_type pos = string.find_first_not_of("0123456789");
		if (pos != std::string::npos)
			return string.substr(0, pos);
		else
			return string;
	}
	return "null";
}
std::string webserv::Regex::match_string(std::string &string)
{
	if (string[0] == '"')
	{
		std::string::size_type pos = string.find("\"", 1);
		if (pos != std::string::npos)
			return string.substr(0, pos + 1);
		else
			throw std::string("Syntax error: missing closing quote");
	}
	else if (string[0] == '\'')
	{
		std::string::size_type pos = string.find("\'", 1);
		if (pos != std::string::npos)
			return string.substr(0, pos + 1);
		else
			throw std::string("Syntax error: missing closing quote");
	}
	else if (isalpha(string[0]) || string[0] == '_')
	{
		std::string::size_type pos = string.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_.");
		if (pos != std::string::npos)
			return string.substr(0, pos);
		else
			return string;
	}
	return "null";
}

//////////////////////////////////////////////////
// Location class
//////////////////////////////////////////////////
webserv::Location::Location() {}
webserv::Location::~Location() {}

void webserv::Location::set_uri(std::string uri) { _uri = uri; }
void webserv::Location::set_root(std::string root) { _root = root; }
void webserv::Location::add_index(std::string index) { _index.push_back(index); }
void webserv::Location::add_allow_methods(std::string allow_methods) { _allow_methods.push_back(allow_methods); }
void webserv::Location::add_return(std::string redirect) { _return.push_back(redirect); }
void webserv::Location::set_autoindex(std::string autoindex) { _autoindex = autoindex; }
void webserv::Location::set_cgi_pass(std::string cgi_pass) { _cgi_pass = cgi_pass; }

std::string webserv::Location::get_uri() const { return _uri; }
std::string webserv::Location::get_root() const { return _root; }
std::vector<std::string> webserv::Location::get_index() const { return _index; }
std::vector<std::string> webserv::Location::get_allow_methods() const { return _allow_methods; }
std::vector<std::string> webserv::Location::get_return() const { return _return; }
std::string webserv::Location::get_autoindex() const { return _autoindex; }
std::string webserv::Location::get_cgi_pass() const { return _cgi_pass; }

//////////////////////////////////////////////////
// Server class
//////////////////////////////////////////////////
webserv::Server::Server() {}
webserv::Server::~Server() {}

void webserv::Server::set_host(std::string host) { _host = host; }
void webserv::Server::set_port(std::string port) { _port = port; }
void webserv::Server::add_server_name(std::string server_name) { _server_name.push_back(server_name); }
void webserv::Server::add_error_page(std::string error_page) { _error_page.push_back(error_page); }
void webserv::Server::set_client_max_body_size(std::string client_max_body_size) { _client_max_body_size = client_max_body_size; }
void webserv::Server::add_location(webserv::Location location) { _locations.push_back(location); }

std::string webserv::Server::get_host()const { return _host; }
std::string webserv::Server::get_port() const { return _port; }
std::vector<std::string> webserv::Server::get_server_name() const { return _server_name; }
std::vector<std::string> webserv::Server::get_error_page() const { return _error_page; }
std::string webserv::Server::get_client_max_body_size() const { return _client_max_body_size; }
std::vector<webserv::Location> webserv::Server::get_locations() const { return _locations; }

//////////////////////////////////////////////////
// Print servers function
//////////////////////////////////////////////////
void webserv::print_vector(std::vector<std::string> vector)
{
	for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
}
void webserv::print_servers(std::vector<webserv::Server> &servers)
{
	std::vector<webserv::Server>::iterator it = servers.begin();

	int i = 1;
	while (it != servers.end())
	{
		std::vector<webserv::Location>::iterator it2 = (*it).get_locations().begin();

		std::cout << "____________________SERVER " << i << "____________________" << std::endl;
		std::cout << "host:			" << it->get_host() << std::endl;
		std::cout << "port:			" << it->get_port() << std::endl;
		std::cout << "server name:		"; webserv::print_vector(it->get_server_name());
		std::cout << "error_page:		"; webserv::print_vector(it->get_error_page());
		std::cout << "client_max_body_size:	" << it->get_client_max_body_size() << std::endl;

		int j = 1;
		while (it2 != (*it).get_locations().end())
		{
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "location: " << j << std::endl;
			std::cout << "	uri:			" << it2->get_uri() << std::endl;
			std::cout << "	root:			" << it2->get_root() << std::endl;
			std::cout << "	index:			"; webserv::print_vector(it2->get_index());
			std::cout << "	allow_methods:		"; webserv::print_vector(it2->get_allow_methods());
			std::cout << "	return:		"; webserv::print_vector(it2->get_return());
			std::cout << "	autoindex:		" << it2->get_autoindex() << std::endl;
			std::cout << "	cgi_pass:		" << it2->get_cgi_pass() << std::endl;

			j++;
			it2++;
		}

		std::cout << "____________________SERVER " << i << "____________________" << std::endl;
		std::cout << std::endl << std::endl << std::endl;
		i++;
		it++;
	}
}
