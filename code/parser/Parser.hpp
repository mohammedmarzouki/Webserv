#if !defined(PARSER_HPP)
#define PARSER_HPP

#include "Tokenizer.hpp"

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

		void parse(std::string);

	};

} // namespace webserv

#endif // PARSER_HPP
