#include "webserv.hpp"

int main(void)
{
	std::string configFile = "   server{listen \"80\";server_name wwwexamplecom;root varwwwhtml;}";
	webserv::Parser parser(configFile);
	parser.parse();
	parser.print_servers();
	return 0;
}
