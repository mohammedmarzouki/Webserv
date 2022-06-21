#if !defined(HANDLE_REQUEST_HPP)
#define HANDLE_REQUEST_HPP

#include <map>

namespace webserv
{
    //////////////////////////////////////////////////
    // Handle_request class
    //////////////////////////////////////////////////
    class Handle_request
    {
    private:
        std::map<int, Request> requests;

    public:
        Handle_request();
        // Handle_request(const Handle_request &);
        // Handle_request &operator=(const Handle_request &);

        void ready_to_recv(int);
        void treat_request(int);
        void ready_to_send(int);
    };

    //////////////////////////////////////////////////
    // Request class
    //////////////////////////////////////////////////
    class Request
    {
    private:
    public:
        Request();
        // Request(const Request &);
        // Request &operator=(const Request &);
    };

    //////////////////////////////////////////////////
    // Response class
    //////////////////////////////////////////////////
    class Response
    {
    private:
    public:
        Response();
        // Response(const Response &);
        // Response &operator=(const Response &);
    };

} // namespace webserv

#endif // HANDLE_REQUEST_HPP
