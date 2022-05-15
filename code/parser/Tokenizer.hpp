#if !defined(TOKENIZER_HPP)
#define TOKENIZER_HPP

#include "../Utils.hpp"

namespace webserv
{
	// Tokenizer class
	class Tokenizer {
		private:
			std::string _input;
			int			_pos;
			std::vector<std::pair<std::string, std::string> > _specs;
		public:
			Tokenizer();
			Tokenizer(std::string);
			~Tokenizer();

			bool isEOF();
			bool hasNext();
			void fillSpecs();
			webserv::Lookahead nextToken();
			std::string match(std::string, std::string);
	};

} // namespace webserv

#endif // TOKENIZER_HPP
