#if !defined(PARSER_HPP)
#define PARSER_HPP

#include "Tokenizer.hpp"

namespace webserv
{
	//////////////////////////////////////////////////
	// Parser class
	//////////////////////////////////////////////////
	class Parser
	{
	private:
		std::string _configFile;
		webserv::Tokenizer _tokenizer;
		webserv::Token _lookahead;
		std::vector<webserv::Server> _servers;

	public:
		Parser();
		~Parser();

		std::vector<webserv::Server> parse(std::string);
		Token eat(std::string);

		webserv::Server server();
		void server_directives(webserv::Server &);
		webserv::Location location();
		void location_directives(webserv::Location &);
	};

} // namespace webserv

#endif // PARSER_HPP
