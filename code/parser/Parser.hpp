#if !defined(PARSER_HPP)
#define PARSER_HPP

#include "Tokenizer.hpp"

namespace webserv
{
	//////////////////////////////////////////////////
	// Parser class
	//////////////////////////////////////////////////
	class Parser {
	private:
		std::string						_configFile;
		webserv::Tokenizer				_tokenizer;
		webserv::Token					_lookahead;
		std::vector<webserv::Server>	_servers;

	public:
		Parser();
		Parser(std::string);
		~Parser();

		void parse();
		Token eat(std::string);

		webserv::Server server();
		void server_derectives(webserv::Server &);

		void print_servers() {
			std::vector<webserv::Server>::iterator it = _servers.begin();
			while (it != _servers.end())
			{
				std::cout << "listen:		" << it->get_listen() << std::endl;
				std::cout << "server name:	" << it->get_server_name() << std::endl;
				std::cout << "root:		" << it->get_root() << std::endl;
				it++;
			}
		}
	};

} // namespace webserv

#endif // PARSER_HPP
