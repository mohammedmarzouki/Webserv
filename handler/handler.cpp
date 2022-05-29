#include "handler.hpp"

sock::SocketMaker::~SocketMaker() {
    // close(new_socket);
}
sock::SocketMaker::SocketMaker(webserv::Server serv) : server(serv) location(serv.get_locations()) {

//     #include <sys/socket.h> 
// … 
// struct sockaddr_in address;
// const int PORT = 8080; //Where the clients can reach at
// /* htonl converts a long integer (e.g. address) to a network representation */ 
// /* htons converts a short integer (e.g. port) to a network representation */ 
// memset((char *)&address, 0, sizeof(address)); 
// address.sin_family = AF_INET; 
// address.sin_addr.s_addr = htonl(INADDR_ANY); 
// address.sin_port = htons(PORT); 
// if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0) 
// { 
//     perror(“bind failed”); 
//     return 0; 
// }



//then


// if (listen(server_fd, 3) < 0) 
// { 
//     perror(“In listen”); 
//     exit(EXIT_FAILURE); 
// }
// if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
// {
//     perror("In accept");            
//     exit(EXIT_FAILURE);        
// }
}