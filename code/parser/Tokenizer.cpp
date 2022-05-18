#include "Tokenizer.hpp"

//////////////////////////////////////////////////
// Tokenizer class
//////////////////////////////////////////////////
webserv::Tokenizer::Tokenizer() {}
webserv::Tokenizer::Tokenizer(std::string input)
	: _input(input), _pos(0) { fill_specs(); }
webserv::Tokenizer::~Tokenizer() {}

void webserv::Tokenizer::fill_specs()
{
	_specs.push_back("whitespace");
	_specs.push_back("comment");

	_specs.push_back("{");
	_specs.push_back("}");
	_specs.push_back(";");

	_specs.push_back("listen");
	_specs.push_back("server_name");
	_specs.push_back("root");
	_specs.push_back("server");

	_specs.push_back("number");
	_specs.push_back("string");
}

bool webserv::Tokenizer::is_EOF() { return _pos >= _input.length(); }
bool webserv::Tokenizer::has_next() { return _pos < _input.length(); }
webserv::Token webserv::Tokenizer::next_token()
{
	if (!has_next())
		return webserv::Token("null", "null");

	std::string string = _input.substr(_pos);

	std::vector<std::string>::iterator it = _specs.begin();
	for (; it != _specs.end(); it++)
	{
		std::string tokenValue = match(*it, string);
		if (tokenValue == "null")
			continue;

		// Should skip token e.g. whitespace or comment
		if (*it == "whitespace" || *it == "comment")
			return webserv::Tokenizer::next_token();

		return webserv::Token(*it, tokenValue);
	}

	throw std::string("Invalid token");
}
std::string webserv::Tokenizer::match(std::string regexp, std::string string)
{
	static webserv::Regex regex;

	std::string matched = regex.match(regexp, string);
	if (matched == "null")
		return "null";
	_pos += matched.length();
	return matched;
}
