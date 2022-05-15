#include "Parser.hpp"

webserv::Parser::Parser() {}

webserv::Parser::~Parser() {}

void webserv::Parser::parse(std::string configFile) {
	this->_tokenizer.init(configFile);

	_lookahead = _tokenizer.nextToken();
	std::cout << _lookahead.getType() << ": " << _lookahead.getValue() << std::endl;
}
