#include "webserv.hpp"

int main(int argc, char **argv)
{
	std::string config_file;
	std::vector<webserv::Server> servers;

	//////////////////////////////////////////////////
	// Initialization
	//////////////////////////////////////////////////
	{
		if (argc == 2)
		{
			config_file = argv[1];
			if (config_file == "--help" || config_file == "-h")
			{
				PRINT("Usage: " << argv[0] << " [configuration file]");
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
				PRINT_ERR("Error: Could not open file " << argv[1]);
				return 1;
			}
			in_stream.close();
		}
		else
		{
			PRINT("Usage: " << argv[0] << " [configuration file]");
			PRINT("Usage: " << argv[0] << " --help [-h]");
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
			PRINT_ERR(err);
		}
	}

	//////////////////////////////////////////////////
	// Processing
	//////////////////////////////////////////////////
	{
	}
	return 0;
}
