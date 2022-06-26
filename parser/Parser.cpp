#include "Parser.hpp"

//////////////////////////////////////////////////
// Parser class
//////////////////////////////////////////////////
Parser::Parser() {}
Parser::Parser(std::string config_file)
{
	_config_file = config_file;
	_tokenizer = Tokenizer(_config_file);
	_lookahead = _tokenizer.next_token();
	while (_lookahead.get_type() == "server")
		_servers.push_back(server());
}
Parser::~Parser() {}

Token Parser::eat(std::string token_type)
{
	Token token = _lookahead;

	if (token.get_type() == "NULL")
		throw std::string("Unexpected end of file");
	if (token.get_type() != token_type)
		throw std::string("Unexpected token: " + token.get_value());
	_lookahead = _tokenizer.next_token();

	return token;
}

Server Parser::server()
{
	Server server;

	eat("server");
	eat("{");
	while (_lookahead.get_value() != "}")
		server_directives(server);
	eat("}");
	return server;
}
void Parser::server_directives(Server &server)
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
		if (_lookahead.get_type() == "number")
		{
			server.add_error_page(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		if (_lookahead.get_type() == "uri")
		{
			server.add_error_page(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "client_max_body_size")
	{
		eat("client_max_body_size");
		if (_lookahead.get_type() == "number")
		{
			server.set_client_max_body_size(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else if (_lookahead.get_type() == "location")
		server.add_location(location());
	else
		throw std::string("Unexpected token: " + _lookahead.get_value());
}

Location Parser::location()
{
	Location location;

	eat("location");
	if (_lookahead.get_type() == "uri")
	{
		location.set_uri(_lookahead.get_value());
		eat(_lookahead.get_type());
	}
	eat("{");
	while (_lookahead.get_value() != "}")
		location_directives(location);
	eat("}");
	return location;
}
void Parser::location_directives(Location &location)
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
	else if (_lookahead.get_type() == "upload")
	{
		eat("upload");
		if (_lookahead.get_type() == "uri")
		{
			location.set_upload(_lookahead.get_value());
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
	else if (_lookahead.get_type() == "cgi_pass")
	{
		eat("cgi_pass");
		if (_lookahead.get_type() == "string")
		{
			location.set_cgi_pass(_lookahead.get_value());
			eat(_lookahead.get_type());
		}
		eat(";");
	}
	else
		throw std::string("Unexpected token: " + _lookahead.get_value());
}

std::vector<Server> Parser::get_servers() const
{
	return _servers;
}
