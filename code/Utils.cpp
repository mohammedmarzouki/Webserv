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

std::string webserv::Regex::match(std::string &regex, std::string &string) {
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

	else if (regex == "number")
		return match_number(string);
	else if (regex == "string")
		return match_string(string);

	else if (regex == "server")
		return match_server(string);
	else if (regex == "listen")
		return match_listen(string);
	else if (regex == "server_name")
		return match_server_name(string);
	else if (regex == "root")
		return match_root(string);
	else
		return "null";
}

std::string webserv::Regex::match_whitespace(std::string &string) {
	if (isspace(string[0])) {
		std::string::size_type pos = string.find_first_not_of(" \t\n");
		if (pos != std::string::npos) return string.substr(0, pos);
		else return string;
	}
	return "null";
}
std::string webserv::Regex::match_comment(std::string &string) {
	if (string[0] == '#') {
		std::string::size_type pos = string.find("\n");
		if (pos != std::string::npos) return string.substr(0, pos);
		else return string;
	}
	return "null";
}

std::string webserv::Regex::match_start_block(std::string &string) {
	if (string[0] == '{')
		return string.substr(0, 1);
	return "null";
}
std::string webserv::Regex::match_end_block(std::string &string) {
	if (string[0] == '}')
		return string.substr(0, 1);
	return "null";
}
std::string webserv::Regex::match_semicolon(std::string &string) {
	if (string[0] == ';')
		return string.substr(0, 1);
	return "null";
}

std::string webserv::Regex::match_number(std::string &string) {
	if (isdigit(string[0])) {
		std::string::size_type pos = string.find_first_not_of("0123456789");
		if (pos != std::string::npos) return string.substr(0, pos);
		else return string;
	}
	return "null";
}
std::string webserv::Regex::match_string(std::string &string) {
	if (string[0] == '"') {
		std::string::size_type pos = string.find("\"", 1);
		if (pos != std::string::npos) return string.substr(0, pos + 1);
		else throw std::string("Syntax error: missing closing quote");
	}
	else if (string[0] == '\'') {
		std::string::size_type pos = string.find("\'", 1);
		if (pos != std::string::npos) return string.substr(0, pos + 1);
		else throw std::string("Syntax error: missing closing quote");
	}
	else if (isalpha(string[0]) || string[0] == '_') {
		std::string::size_type pos = string.find_first_not_of("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
		if (pos != std::string::npos) return string.substr(0, pos);
		else return string;
	}
	return "null";
}

std::string webserv::Regex::match_server(std::string &string) {
	std::string server = string.substr(0, 6);
	if (server == "server") return server;
	return "null";
}
std::string webserv::Regex::match_listen(std::string &string) {
	std::string listen = string.substr(0, 6);
	if (listen == "listen") return listen;
	return "null";
}
std::string webserv::Regex::match_server_name(std::string &string) {
	std::string server_name = string.substr(0, 11);
	if (server_name == "server_name") return server_name;
	return "null";
}
std::string webserv::Regex::match_root(std::string &string) {
	std::string match_root = string.substr(0, 10);
	if (match_root == "match_root") return match_root;
	return "null";
}

//////////////////////////////////////////////////
// Server class
//////////////////////////////////////////////////
webserv::Server::Server() {}
webserv::Server::~Server() {}

std::string webserv::Server::get_listen() { return _listen; }
std::string webserv::Server::get_server_name() { return _server_name; }
std::string webserv::Server::get_root() { return _root; }
std::string webserv::Server::get_error_page() { return _error_page; }

void	webserv::Server::set_listen(std::string listen) { _listen = listen; }
void	webserv::Server::set_server_name(std::string server_name) { _server_name = server_name; }
void	webserv::Server::set_root(std::string root) { _root = root; }
void	webserv::Server::set_error_page(std::string error_page) { _error_page = error_page; }
