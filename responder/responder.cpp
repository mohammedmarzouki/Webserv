#include "responder.hpp"
#include <sstream>

// std::string &AutoindexMaker(std::string &path){
//   std::string html = "<html><head><title>Index of " + path + "</title></head><body><h1>Index of "
//                     + path + "</h1><hr><pre>\n";

//   op
//   html += "\n</pre><hr></body>\n</html>";
// }

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
  case MOVED_PERMANENTLY:
    return ("301 MOVED_PERMANENTLY");
    break;
  case FOUND:
    return ("302 FOUND");
    break;
  case TEMPORARY_REDIRECT:
    return ("307 TEMPORARY_REDIRECT");
    break;
  case PERMANENT_REDIRECT:
    return ("308 PERMANENT_REDIRECT");
    break;
  case BAD_REQUEST:
    return ("400 BAD_REQUEST");
    break;
  case FORBIDDEN:
    return ("403 FORBIDDEN");
    break;
  case METHOD_NOT_ALLOWED:
    return ("405 METHOD_NOT_ALLOWED");
    break;
  case NOT_FOUND:
    return ("404 NOT_FOUND");
    break;
  case INTERNAL_SERVER_ERROR:
    return ("500 INTERNAL_SERVER_ERROR");
    break;
  case BAD_GATEWAY:
    return ("502 BAD_GATEWAY");
    break;
  default:
    std::stringstream ss; 
    std::string str; 
    ss << i;  
    ss >> str; 
    return (str);
}
}


