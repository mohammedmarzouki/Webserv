#include "parser/Parser.hpp"

int main(void)
{
	webserv::Parser parser;
	parser.parse("   #hhhh\n{Hello asdf  ;  }#ggggg");
	return 0;
}
