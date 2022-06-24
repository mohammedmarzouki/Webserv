#include "../webserv.hpp"

void setup_config_file(int argc, char **argv, std::string &config_file)
{
	if (argc == 2)
	{
		config_file = argv[1];
		// ./webserv --help || -h
		if (config_file == "--help" || config_file == "-h")
			PRINT("Usage: " << argv[0] << " [configuration file]");
		// ./webserv [configuration file]
		else
		{
			read_config_file(config_file);
			return;
		}
	}
	// ./webserv || ./webserv param1 param2 ...
	else
	{
		PRINT("Usage: " << argv[0] << " [configuration file]");
		PRINT("Usage: " << argv[0] << " --help [-h]");
	}
	exit(EXIT_SUCCESS);
}

void read_config_file(std::string &config_file)
{
	std::ifstream in_stream(config_file.c_str());
	std::string line;

	if (!in_stream)
	{
		PRINT_ERR("Error: Could not open file " << config_file);
		exit(EXIT_FAILURE);
	}
	config_file.clear();
	while (std::getline(in_stream, line))
		config_file += line + "\n";
	in_stream.close();
}

void exit_err(std::string &err)
{
	PRINT_ERR(err);
	exit(EXIT_FAILURE);
}

std::vector<std::string> split_string(std::string str, std::string delimiter)
{
	(void)delimiter;
	std::stringstream stream(str);
	std::string word;
	std::vector<std::string> final_vector;

	while (stream >> word)
		final_vector.push_back(word);
	return final_vector;
}
