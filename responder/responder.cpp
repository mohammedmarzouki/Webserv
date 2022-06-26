#include "responder.hpp"


std::string &headerMaker(Response &rp){
    std::string resp;

    std::string newline = "\n\r";

    resp.append("HTTP/1.1 ");
    resp.append(rp.get_status_line());
    resp.append(newline);
    resp.append("Content-Length: ");
    resp.append(rp.get_content_length());
    resp.append("Content-Type: ");
    resp.append(rp.get_content_type());
    resp.append("Connection: ");
    resp.append(rp.get_connection());
    resp.append(newline);
    resp.append(newline);
    return (resp);
}

std::string statusMaker(int &i){
switch (i) {
  case OK:
    return ("200 OK");
    break;
  case CONTINUE:
    return ("100 CONTINUE");
    break;
  case SWITCHING_PROTOCOLS:
    return ("101 SWITCHING PROTOCOLS");
    break;
  case CREATED:
    return ("201 CREATED");
    break;
}//default atoi
}


