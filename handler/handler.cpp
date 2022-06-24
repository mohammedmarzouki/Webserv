#include "handler.hpp"


sock::SocketMaker::~SocketMaker() {
    close(_socket);
}
sock::SocketMaker::SocketMaker(webserv::Server &serv) : server(serv) , location(serv.get_locations()) {
	struct sockaddr_in address;

	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(serv.get_port());
	address.sin_addr.s_addr = inet_addr((serv.get_host()).c_str());

	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw "failed socket";
	if(fcntl(_socket, F_SETFL, O_NONBLOCK))
		throw "failed fcntl";
	
	int bool_opt(1);
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &bool_opt, sizeof(bool_opt)))
		throw "failed socketoptions";

    if (bind(_socket, (sockaddr *) &address,  sizeof(address)))
		throw "failed bind socket";

	if(listen(_socket, 1024))
		throw "failed listening to socket";

}

int sock::set_servers(fd_set &rd,fd_set &wr){
	int max_fd(0);

	FD_ZERO(&rd);
	FD_ZERO(&wr);
	for (size_t i = 0; i < srv.size(); i++){
		FD_SET(srv[i]._socket,&rd);
		max_fd = max_fd < srv[i]._socket ? srv[i]._socket : max_fd;
	}
	return (max_fd);
}

bool sock::isserver(const int &fd){
	for(size_t i(0); i < srv.size(); i++){
		if(srv[i]._socket == fd)
			return(true);
	}
	return(false);
}

webserv::Server &sock::search_server(const int &fd){
	for(size_t i(0); i < srv.size(); i++){
		if(srv[i]._socket == fd)
			return(srv[i].server);
	}
	return(srv[0].server);
}

void sock::ReadyToRead(int &fd, fd_set &rd,fd_set &wr, int &max_fd){
	if (isserver(fd)){
		int client;
		if((client = accept(fd, NULL, NULL)) < 0)
			return ;
		fcntl(client, F_SETFL, O_NONBLOCK);
		cli_srv[client] = fd;
		FD_SET(client, &rd);
		max_fd = max_fd < client ? client : max_fd;

	} else {
		switch (handler.recv_request(fd, search_server(cli_srv[fd]))) {
			case FAILED:
				FD_CLR(fd, &rd);
				cli_srv.erase(fd);
				close(fd);
				fd == max_fd ? max_fd-- : 0;
				break;
			case DONE:
				FD_CLR(fd, &rd);
				FD_SET(fd, &wr);
				break;
		}
	}
}
void sock::ReadyToWrite(int &fd, fd_set &rd,fd_set &wr, int &max_fd){

	switch (handler.send_response(fd)) {
		case KILL_CONNECTION:
			FD_CLR(fd, &wr);
			cli_srv.erase(fd);
			close(fd);
			fd == max_fd ? max_fd-- : 0;
			break;
		case KEEP_ALIVE:
			FD_CLR(fd, &wr);
			FD_SET(fd, &rd);
			break;
	}

}

void sock::looper(std::vector<webserv::Server> servers){
	for(size_t i(0); i < servers.size(); i++){
		try {
			srv.push_back(sock::SocketMaker(servers[i]));
		}catch(const std::string &e) {
			PRINT_ERR(e);
			srv.clear();
			return;
		}
	}
	fd_set rd , wr;
	int max_fd = set_servers(rd, wr);
	
	while (1) {
		if(select(max_fd + 1, &rd, &wr, NULL,  NULL) < 1)
			continue;
		for (int i(0); i < max_fd; i++){
			if(FD_ISSET(i, &rd))
				ReadyToRead(i, rd, wr, max_fd);
			else if (FD_ISSET(i, &wr))
				ReadyToWrite(i, rd, wr, max_fd);
		}
	}
	
}