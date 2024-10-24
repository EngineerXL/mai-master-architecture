#ifndef HTTPREQUESTFACTORY_HPP
#define HTTPREQUESTFACTORY_HPP

#include <iostream>

#include "../handlers/user_handler.hpp"

class HTTPRequestFactory : public HTTPRequestHandlerFactory {
   public:
    HTTPRequestFactory(const std::string& format) : _format(format) {}

    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
        std::cout << "request:" << request.getURI() << std::endl;
        if (request.getURI().starts_with("/user"))
            return new UserHandler(_format);
        return 0;
    }

   private:
    std::string _format;
};

#endif /* HTTPREQUESTFACTORY_HPP */
