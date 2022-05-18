#include "webserv.hpp"

int main(void)
{
	// std::string configFile;
	// std::vector<webserv::Server> _servers;
	// {
	// 	// Init
	// } {
	// 	// Parsing
	// } {
	// 	// Processing
	// } {
		std::vector<webserv::Server> servers;
		std::string configFile = "   server{listen \"80\";server_name wwwexamplecom;    root varwwwhtml;    }";
		std::string configFile2 = "   server{listen \"8000\";server_name wwwwwwwwww;    root varvarvar;    }";
		webserv::Parser parser;
		servers = parser.parse(configFile);
		webserv::print_servers(servers);
		servers = parser.parse(configFile2);
		webserv::print_servers(servers);
	// }
	return 0;
}
