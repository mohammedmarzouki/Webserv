#include "../webserv.hpp"

void	print_help(int argc , char **argv, std::string &file) {

	if (argc == 2)
        file = argv[1];
	if (argc == 2 && (file == "--help" || file == "-h"))
		PRINT("Usage: " << argv[0] << " [configuration file]");
	else if (argc == 2)
        return ;
    else {
		PRINT("Usage: " << argv[0] << " [configuration file]");
		PRINT("Usage: " << argv[0] << " --help [-h]");
	}
	exit(EXIT_SUCCESS);
}

void	exit_err(std::string err) {
	PRINT_ERR(err);
	exit(EXIT_FAILURE);
}

void	read_config(std::string &config_file) {
	std::ifstream in_stream(config_file.c_str());

	if (!in_stream){
        PRINT_ERR("Error: Could not open file " << config_file.c_str());
	    exit(EXIT_FAILURE);
    }
	config_file.clear();
	std::string line;
	while (std::getline(in_stream, line))
		config_file += line + "\n";

	in_stream.close();
}