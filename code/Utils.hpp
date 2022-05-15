#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <iostream>
#include <vector>

namespace webserv
{
	// Lookahead class
	class Lookahead {
		private:
			std::string _type;
			std::string _value;
		public:
			Lookahead();
			Lookahead(std::string, std::string);
			~Lookahead();

			std::string getType();
			std::string getValue();
			void setType(std::string type);
			void setValue(std::string value);
	};

	// Regex class
	class Regex {
		public:
			Regex();
			~Regex();

			std::string match(std::string &, std::string &);
			std::string matchWhitespace(std::string &);
			std::string matchComment(std::string &);
			std::string matchDirective(std::string &);
			std::string matchBlock(std::string &);
	};

	// Server class
	class Server {};

	// Location class
	class Location {};

} // namespace webserv

#endif // UTILS_HPP
