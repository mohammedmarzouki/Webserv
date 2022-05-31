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
	if (_lookahead.get_type() == "host")
	{
		eat("host");
		if (_lookahead.get_type() == "string" || _lookahead.get_type() == "ipv4")
		{
			server.set_host(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "port")
	{
		eat("port");
		if (_lookahead.get_type() == "number")
		{
			server.set_port(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "server_name")
	{
		eat("server_name");
		while (_lookahead.get_type() != ";" && _lookahead.get_type() == "string")
		{
			server.add_server_name(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "error_page")
	{
		eat("error_page");
		while (_lookahead.get_value() != ";" && (_lookahead.get_type() == "number" || _lookahead.get_type() == "uri"))
		{
			server.add_error_page(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "client_max_body_size")
	{
		std::string size;

		eat("client_max_body_size");
		if (_lookahead.get_type() == "number")
		{
			size = _lookahead.get_value();
			eat(_lookahead.get_type());
			if (_lookahead.get_type() == "string")
				size += _lookahead.get_value();
			server.set_client_max_body_size(size);
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
	eat("uri");
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
		if (_lookahead.get_type() == "uri")
		{
			location.set_root(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "index")
	{
		eat("index");
		while (_lookahead.get_value() != ";" && _lookahead.get_type() == "string")
		{
			location.add_index(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "allow_methods")
	{
		eat("allow_methods");
		while (_lookahead.get_value() != ";" && _lookahead.get_type() == "string")
		{
			location.add_allow_methods(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "redirect")
	{
		eat("redirect");
		while (_lookahead.get_value() != ";" && (_lookahead.get_type() == "number" || _lookahead.get_type() == "string"))
		{
			location.add_return(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "autoindex")
	{
		eat("autoindex");
		if (_lookahead.get_type() == "string")
		{
			location.set_autoindex(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else
		throw std::string("Unexpected token: " + _lookahead.get_value());
}
