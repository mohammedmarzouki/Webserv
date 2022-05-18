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
	try
	{
		std::vector<webserv::Server> servers;
		std::string configFile = "\
		server {\
			listen \"80\";\
			server_name wwwexamplecom;\
			root varwwwhtml;\
			location {\
				root secondseconde;\
				path thisthis;\
			}\
			location {\
				root third_third;\
				path thatthat;\
			}\
		}";
		webserv::Parser parser;
		servers = parser.parse(configFile);
		webserv::print_servers(servers);
	}
	catch (std::string &err)
	{
		std::cout << err << std::endl;
	}
	// }
	return 0;
}
