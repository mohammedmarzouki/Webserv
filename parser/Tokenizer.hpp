#if !defined(TOKENIZER_HPP)
#define TOKENIZER_HPP

#include "Utils.hpp"

namespace webserv
{
	//////////////////////////////////////////////////
	// Tokenizer class
	//////////////////////////////////////////////////
	class Tokenizer
	{
	private:
		std::string _input;
		size_t _pos;
		std::vector<std::string> _specs;

	public:
		Tokenizer();
		Tokenizer(std::string);
		~Tokenizer();

		void fill_specs();

		bool is_EOF();
		bool has_next();
		webserv::Token next_token();
		std::string match(std::string, std::string);
	};

} // namespace webserv

#endif // TOKENIZER_HPP
