#ifndef CGI_HPP
#define CGI_HPP

#include "../handler/handler.hpp"

class cgi
{
private:
	int _status;
	std::string outfile;
	int infd;
	int outfd;
	int pid;
	char *args[3];
	Request &request ;
	Response &response ;

	int	MakeRespFile(int &fd);
	int checktype(std::string path);
	void execute_cgi();
	void SetStatus(int stat);
	void SetCgiEnv();
	void SetOutFile(std::string name);
public:
	cgi(int fd);
	~cgi();
	int GetStatus();
	int GetOutFile();
};

#endif