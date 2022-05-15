#include "Tokenizer.hpp"

// Tokenizer class
webserv::Tokenizer::Tokenizer(): _input(""), _pos(0) {}

webserv::Tokenizer::Tokenizer(std::string input): _input(input), _pos(0) {
	fillSpecs();
}

webserv::Tokenizer::~Tokenizer() {}

bool webserv::Tokenizer::isEOF() {
	return _pos >= _input.length();
}

bool webserv::Tokenizer::hasNext() {
	return _pos < _input.length();
}

void webserv::Tokenizer::fillSpecs() {
	// whitespace
	_specs.push_back(std::make_pair("whitespace", "null"));
	// Single line comment
	_specs.push_back(std::make_pair("comment", "null"));
	// block
	_specs.push_back(std::make_pair("block", "block"));
	// directive
	_specs.push_back(std::make_pair("directive", "directive"));
}

webserv::Lookahead webserv::Tokenizer::nextToken()
{
	if (!hasNext())
		return webserv::Lookahead("end", "end");
	std::string string = _input.substr(_pos);

	std::vector<std::pair<std::string, std::string> >::iterator it = _specs.begin();
	for (; it != _specs.end(); it++)
	{
		std::string tokenValue = match(it->first, string);
		if (tokenValue == "null")
			continue;

		// Should skip token e.g. whitespace or comment
		if (it->second == "null")
			return webserv::Tokenizer::nextToken();

		return webserv::Lookahead(it->second, tokenValue);
	}

	return webserv::Lookahead("not", "not");
	// throw new std::exception("Invalid token");
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
