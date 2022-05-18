#include "webserv.hpp"

int main(int argc, char **argv)
{
	std::string config_file(argv[1]);
	std::vector<webserv::Server> servers;

	//////////////////////////////////////////////////
	// Initialization
	//////////////////////////////////////////////////
	{
		if (argc == 2)
		{
			if (config_file == "--help" || config_file == "-h")
			{
				std::cout << "Usage: " << argv[0] << " [configuration file]" << std::endl;
				return 0;
			}
			std::ifstream in_stream(config_file.c_str());
			if (in_stream)
			{
				config_file.clear();
				std::string line;
				while (std::getline(in_stream, line))
					config_file += line + "\n";
			}
			else
			{
				std::cerr << "Error: Could not open file " << argv[1] << std::endl;
				return 1;
			}
			in_stream.close();
		}
		else
		{
			std::cout << "Usage: " << argv[0] << " [configuration file]" << std::endl;
			std::cout << "Usage: " << argv[0] << " --help [-h]" << std::endl;
		}
	}
	//////////////////////////////////////////////////
	// Parsing
	//////////////////////////////////////////////////
	{
		webserv::Parser parser;
		try
		{
			servers = parser.parse(config_file);
			webserv::print_servers(servers);
		}
		catch (std::string &err)
		{
			std::cout << err << std::endl;
		}
	}
	// {
	// Processing
	// }
	return 0;
}
