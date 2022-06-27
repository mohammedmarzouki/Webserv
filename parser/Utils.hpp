#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>

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
	Token(const Token &);
	~Token();

	Token &operator=(const Token &);

	void set_type(std::string);
	void set_value(std::string);
	std::string get_type() const;
	std::string get_value() const;
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
	std::string match_host(std::string &);
	std::string match_port(std::string &);
	std::string match_server_name(std::string &);
	std::string match_error_page(std::string &);
	std::string match_client_max_body_size(std::string &);

	std::string match_location(std::string &);
	std::string match_root(std::string &);
	std::string match_index(std::string &);
	std::string match_allow_methods(std::string &);
	std::string match_return(std::string &);
	std::string match_upload(std::string &);
	std::string match_autoindex(std::string &);
	std::string match_cgi_pass(std::string &);

	std::string match_ipv4(std::string &);
	std::string match_uri(std::string &);
	std::string match_number(std::string &);
	std::string match_string(std::string &);
};

//////////////////////////////////////////////////
// Location class
//////////////////////////////////////////////////
class Location
{
private:
	std::string _uri;
	std::string _root;
	std::vector<std::string> _index;
	std::vector<std::string> _allow_methods;
	std::vector<std::string> _return;
	std::string _upload;
	std::string _autoindex;
	std::string _cgi_pass;

public:
	Location();
	Location(const Location &);
	~Location();

	Location &operator=(const Location &);

	void set_uri(std::string);
	void set_root(std::string);
	void add_index(std::string);
	void add_allow_methods(std::string);
	void add_return(std::string);
	void set_upload(std::string);
	void set_autoindex(std::string);
	void set_cgi_pass(std::string);
	std::string get_uri() const;
	std::string get_root() const;
	std::vector<std::string> get_index() const;
	std::vector<std::string> get_allow_methods() const;
	std::vector<std::string> get_return() const;
	std::string get_upload() const;
	std::string get_autoindex() const;
	std::string get_cgi_pass() const;
};

//////////////////////////////////////////////////
// Server class
//////////////////////////////////////////////////
class Server
{
private:
	std::string _host;
	short _port;
	std::vector<std::string> _server_name;
	std::vector<std::string> _error_page;
	int _client_max_body_size;
	std::vector<Location> _locations;

public:
	Server();
	Server(const Server &);
	~Server();

	Server &operator=(const Server &);

	void set_host(std::string);
	void set_port(std::string);
	void add_server_name(std::string);
	void add_error_page(std::string);
	void set_client_max_body_size(std::string);
	void add_location(Location);
	std::string get_host() const;
	short get_port() const;
	std::vector<std::string> get_server_name() const;
	std::vector<std::string> get_error_page() const;
	int get_client_max_body_size() const;
	std::vector<Location> get_locations() const;
};

//////////////////////////////////////////////////
// Print servers function
//////////////////////////////////////////////////
void print_servers(std::vector<Server> &);
void print_vector(std::vector<std::string>);

#endif // UTILS_HPP
