#include "Parser.hpp"

//////////////////////////////////////////////////
// Parser class
//////////////////////////////////////////////////
webserv::Parser::Parser() {}
webserv::Parser::~Parser() {}

std::vector<webserv::Server> webserv::Parser::parse(std::string configFile)
{
	_configFile = configFile;
	_tokenizer = webserv::Tokenizer(_configFile);
	_lookahead = _tokenizer.next_token();
	_servers.clear();
	while (_lookahead.get_type() == "server")
		_servers.push_back(server());
	return _servers;
}
webserv::Token webserv::Parser::eat(std::string token_type)
{
	webserv::Token token = _lookahead;

	if (token.get_type() == "null")
		throw std::string("Unexpected end of file");
	if (token.get_type() != token_type)
		throw std::string("Unexpected token: " + token.get_value());
	_lookahead = _tokenizer.next_token();

	return token;
}

webserv::Server webserv::Parser::server()
{
	webserv::Server server;

	eat("server");
	eat("{");
	while (_lookahead.get_value() != "}")
		server_directives(server);
	eat("}");
	return server;
}
void webserv::Parser::server_directives(webserv::Server &server)
{
	if (_lookahead.get_type() == "listen")
	{
		eat("listen");
		if (_lookahead.get_type() == "string")
		{
			server.set_listen(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "server_name")
	{
		eat("server_name");
		if (_lookahead.get_type() == "string")
		{
			server.set_server_name(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "root")
	{
		eat("root");
		if (_lookahead.get_type() == "string")
		{
			server.set_root(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "location")
		server.add_location(location());
	else
		throw std::string("Unexpected token: " + _lookahead.get_value());
}

webserv::Location webserv::Parser::location()
{
	webserv::Location location;

	eat("location");
	eat("{");
	while (_lookahead.get_value() != "}")
		location_directives(location);
	eat("}");
	return location;
}

void webserv::Parser::location_directives(webserv::Location &location)
{
	if (_lookahead.get_type() == "root")
	{
		eat("root");
		if (_lookahead.get_type() == "string")
		{
			location.set_root(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "path")
	{
		eat("path");
		if (_lookahead.get_type() == "string")
		{
			location.set_path(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else
		throw std::string("Unexpected token: " + _lookahead.get_value());
}
