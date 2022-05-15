#include "Parser.hpp"

webserv::Parser::Parser() {
	this->_configFile = "";
	this->_tokenizer = webserv::Tokenizer();
}

webserv::Parser::~Parser() {}

std::vector<Server> webserv::Parser::parse(std::string configFile) {
	this->_configFile = configFile;
	this->_tokenizer.init(this->_configFile);

	return this->program();
}

std::vector<Server> program(void) {}
