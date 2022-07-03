#include <sstream>
#include <stdlib.h>
#include "cgi.hpp"

int	cgi::MakeRespFile(int &fd){
	std::stringstream s;
	std::string tmp;
	s << fd , s >> tmp;
	int ret = open(("/tmp/CgiResp" + tmp).c_str(), O_CREAT | O_WRONLY | O_TRUNC , 0666);

	return (ret);
}

int cgi::checktype(std::string path) {
	struct stat s;
	if(stat(path.c_str(), &s))
		return (0);
	if (S_ISDIR(s.st_mode))
		return(2);
	if (!S_ISREG(s.st_mode))
		return (0);
	if(!(s.st_mode & S_IXUSR))
		return (0);
	return (1);
}
void cgi::SetCgiEnv() {
	setenv("GATEWAY_INTERFACE", "CGI/1.1",1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1",1);
	setenv("REDIRECT_STATUS", "200",      1);
	setenv("SCRIPT_NAME", args[1] , 1);
	setenv("REQUEST_METHOD",  request.get_method().c_str(), 1);
	setenv("CONTENT_TYPE", request.get_content_type().c_str(), 1);
	if (request.get_method() == "GET") {
		setenv("QUERY_STRING", "", 1);
		setenv("CONTENT_LENGTH", "", 1);
	}
	else if (request.get_method() == "POST") {
		infd = open(request.get_path_to_upload().c_str(), O_RDONLY, 0666);
		std::stringstream s;
		std::string tmp;
		s << request.get_content_length() , s >> tmp;
		setenv("QUERY_STRING", "", 1);
		setenv("CONTENT_LENGTH", "tmp", 1);
	}
}

void	cgi::execute_cgi(){
	char** env;
	pid = fork();
	if (pid < 0)
		return;
	if (pid == 0) {
		SetCgiEnv();
		dup2(infd, 0);
		dup2(outfd, 1);
		execve(args[0], (char **)args, env);
		exit(1);
	}
}

cgi::cgi(int fd) :  infd(0), _status(0), request(handler.requests[fd].first), response(handler.requests[fd].second) {
	request.get_method();
	outfd = MakeRespFile(fd);
	if (outfd < 0 || checktype(response.get_cgi_path()) != 1){
		SetStatus(500);
		return;
	}
	char  *inputfile = (char*)request.get_path().c_str();
	args[0] = (char *)response.get_cgi_path().c_str();
	args[1] = (char *)inputfile;
	args[2] = NULL;
	execute_cgi();
}

int	cgi::GetStatus(){
	int Stat;
	int ret;

	if(_status)
		return (_status);
	ret = waitpid(pid, &Stat, WNOHANG);
	if(!ret)
		return(0);//not finished
	if (infd > 0)
		close(infd);
	close(outfd);
	if(ret == pid)
		return(1);//done
	return(500);//server error
}

cgi::~cgi() {}
int cgi(int fd){
}