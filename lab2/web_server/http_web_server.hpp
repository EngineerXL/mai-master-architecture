#ifndef HTTPWEBSERVER_HPP
#define HTTPWEBSERVER_HPP

#include "../database/user.hpp"
#include "http_request_factory.hpp"

class HTTPWebServer : public Poco::Util::ServerApplication {
   public:
    int main([[maybe_unused]] const std::vector<std::string> &args) {
        database::User::init();
        ServerSocket svs(Poco::Net::SocketAddress("0.0.0.0", 8080));
        HTTPServer srv(new HTTPRequestFactory(DateTimeFormat::SORTABLE_FORMAT),
                       svs, new HTTPServerParams);
        srv.start();
        waitForTerminationRequest();
        srv.stop();

        return Application::EXIT_OK;
    }
};
#endif /* HTTPWEBSERVER_HPP */
