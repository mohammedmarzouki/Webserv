#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>

namespace webserv
{
	//////////////////////////////////////////////////
	// Token class
	//////////////////////////////////////////////////
	class Token
	{
	private:
		std::string _type;
		std::string _value;

	public:
		Token();
		Token(std::string, std::string);
		~Token();

		void set_type(std::string);
		void set_value(std::string);
		std::string get_type();
		std::string get_value();
	};

	//////////////////////////////////////////////////
	// Regex class
	//////////////////////////////////////////////////
	class Regex
	{
	public:
		Regex();
		~Regex();

		std::string match(std::string &, std::string &);
		std::string match_whitespace(std::string &);
		std::string match_comment(std::string &);

		std::string match_start_block(std::string &);
		std::string match_end_block(std::string &);
		std::string match_semicolon(std::string &);

		std::string match_server(std::string &);
		std::string match_listen(std::string &);
		std::string match_server_name(std::string &);
		std::string match_root(std::string &);

		std::string match_location(std::string &);
		std::string match_path(std::string &);

		std::string match_number(std::string &);
		std::string match_string(std::string &);
	};

	//////////////////////////////////////////////////
	// Location class
	//////////////////////////////////////////////////
	class Location
	{
	private:
		std::string _path;
		std::string _root;

	public:
		Location();
		~Location();

		void set_path(std::string);
		void set_root(std::string);
		std::string get_path();
		std::string get_root();
	};

	//////////////////////////////////////////////////
	// Server class
	//////////////////////////////////////////////////
	class Server
	{
	private:
		std::string _listen;
		std::string _server_name;
		std::string _root;
		std::vector<Location> _locations;

	public:
		Server();
		~Server();

		void set_listen(std::string);
		void set_server_name(std::string);
		void set_root(std::string);
		void add_location(Location);
		std::string get_listen();
		std::string get_server_name();
		std::string get_root();
		std::vector<Location> get_locations();
	};

	//////////////////////////////////////////////////
	// Print servers function
	//////////////////////////////////////////////////
	void print_servers(std::vector<Server> &);

} // namespace webserv

#endif // UTILS_HPP
