#include "Utils.hpp"

// Lookahead class
webserv::Lookahead::Lookahead()
{
	// std::cout << "Lookahead default constructor called" << std::endl;
	_type = "";
	_value = "";
}

webserv::Lookahead::Lookahead(std::string type, std::string value) {
	// std::cout << "Lookahead constructor called" << std::endl;
	_type = type;
	_value = value;
}

webserv::Lookahead::~Lookahead() {
	// std::cout << "Lookahead destructor called" << std::endl;
}

std::string webserv::Lookahead::getType() {
	return _type;
}

std::string webserv::Lookahead::getValue() {
	return _value;
}

void webserv::Lookahead::setType(std::string type) {
	_type = type;
}

void webserv::Lookahead::setValue(std::string value) {
	_value = value;
}

// Regex class
webserv::Regex::Regex() {}

webserv::Regex::~Regex() {}

std::string webserv::Regex::match(std::string &regex, std::string &string) {
	if (regex == "whitespace")
		return matchWhitespace(string);
	else if (regex == "comment")
		return matchComment(string);
	else if (regex == "block")
		return matchBlock(string);
	else if (regex == "directive")
		return matchDirective(string);
	else
		return "null";
}

std::string webserv::Regex::matchWhitespace(std::string &string) {
	if (string[0] == ' ' || string[0] == '\t' || string[0] == '\n' || string[0] == '\r') {
		std::string whitespace = " \t\n\r";
		std::string::size_type pos = string.find_first_not_of(whitespace);
		if (pos != 0) {
			std::cout << "Whitespace found: " << "|" << string.substr(0, pos) << "|" << std::endl;
			return string.substr(0, pos);
		}
	}
	return "null";
}

std::string webserv::Regex::matchComment(std::string &string) {
	if (string[0] == '#') {
		std::string::size_type pos = string.find("\n");
		std::cout << "Comment found: " << "|" << string.substr(0, pos) << "\\n|" << std::endl;
		return string.substr(0, pos + 1);
	}
	return "null";
}

std::string webserv::Regex::matchBlock(std::string &string) {
	if (string[0] == '{') {
		std::string::size_type pos = string.find("}");
		if (pos != std::string::npos) {
			std::cout << "Block found: " << "|" << string.substr(0, pos + 1) << "|" << std::endl;
			return string.substr(0, pos + 1);
		}
	}
	return "null";
}

std::string webserv::Regex::matchDirective(std::string &string) {
	if (isalpha(string[0])) {
		std::string::size_type pos = string.find(";");
		if (pos != std::string::npos) {
			std::cout << "Directive found: " << "|" << string.substr(0, pos + 1) << "|" << std::endl;
			return string.substr(0, pos + 1);
		}
	}
	return "null";
}
