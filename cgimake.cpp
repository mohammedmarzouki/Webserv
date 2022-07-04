#include <iostream>

int main() {
    std::cout << "HTTP/1.1 200 OK\r\n\
                Content-Length: 88\r\n\
                Content-Type: text/html\r\n\
                Connection: keep-alive\r\n\
                \r\n\
                <html>\
                <body>\
                <h1>From PHP CGI!</h1>\
                </body>\
                </html>"
            << std::endl;
    return (0);
}
