#if !defined(TOKENIZER_HPP)
#define TOKENIZER_HPP

#include "../Utils.hpp"

namespace webserv
{
	// Tokenizer class
	class Tokenizer {
		private:
			std::string _input;
			size_t		_pos;
			std::vector<std::pair<std::string, std::string> > _specs;
		public:
			Tokenizer();
			~Tokenizer();

			void init(std::string);
			bool isEOF();
			bool hasNext();
			void fillSpecs();
			webserv::Lookahead nextToken();
			std::string match(std::string, std::string);
	};

} // namespace webserv

#endif // TOKENIZER_HPP
