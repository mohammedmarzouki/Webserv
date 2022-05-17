#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <iostream>
#include <vector>

namespace webserv
{
	//////////////////////////////////////////////////
	// Token class
	//////////////////////////////////////////////////
	class Token {
		private:
			std::string	_type;
			std::string	_value;

		public:
			Token();
			Token(std::string, std::string);
			~Token();

			void set_type(std::string);
			void set_value(std::string);
			std::string get_type();
			std::string get_value();
	};

	//////////////////////////////////////////////////
	// Regex class
	//////////////////////////////////////////////////
	class Regex {
		public:
			Regex();
			~Regex();

			std::string match(std::string &, std::string &);
			std::string match_whitespace(std::string &);
			std::string match_comment(std::string &);

			std::string match_start_block(std::string &);
			std::string match_end_block(std::string &);
			std::string match_semicolon(std::string &);

			std::string match_server(std::string &);
			std::string match_listen(std::string &);
			std::string match_server_name(std::string &);
			std::string match_root(std::string &);

			std::string match_number(std::string &);
			std::string match_string(std::string &);
	};

	//////////////////////////////////////////////////
	// Server class
	//////////////////////////////////////////////////
	class Server {
		private:
			std::string				_listen;
			std::string				_server_name;
			std::string				_root;
			std::string				_error_page;

		public:
			Server();
			~Server();

			void set_listen(std::string);
			void set_server_name(std::string);
			void set_root(std::string);
			std::string get_listen();
			std::string get_server_name();
			std::string get_root();
	};

	//////////////////////////////////////////////////
	// Location class
	//////////////////////////////////////////////////
	class Location {};

} // namespace webserv

#endif // UTILS_HPP
