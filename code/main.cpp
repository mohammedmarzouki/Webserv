#include "parser/Tokenizer.hpp"

int main(void)
{
	webserv::Tokenizer tokenizer(
	"   #hhhh\n{Hello asdf  ;  }#ggggg");
	webserv::Lookahead lookahead = tokenizer.nextToken();

	std::cout << lookahead.getType() << ": " << lookahead.getValue() << std::endl;
	return 0;
}
