#include <iostream>

int main()
{
    std::cout << "HTTP/1.1 200 OK\r\nContent-Length: 51\r\nContent-Type: text/html\r\nConnection: keep-alive\r\n\r\n\
<html><body><h1>From Python CGI!</h1></body></html>"
              << std::endl;
    return (0);
}
