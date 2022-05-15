#if !defined(PARSER_HPP)
#define PARSER_HPP

#include <iostream>
#include <vector>
#include "Tokenizer.hpp"
#include "../Server.hpp"

namespace webserv
{
	class Parser {
	private:
		std::string _configFile;
		webserv::Tokenizer _tokenizer;
		webserv::Lookahead _lookahead;
		std::vector<webserv::Server> servers;

	public:
		Parser();
		~Parser();

		std::vector<Server> parse(std::string);
		std::vector<Server> program(void);

	};

} // namespace webserv

#endif // PARSER_HPP
