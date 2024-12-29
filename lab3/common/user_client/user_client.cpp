#include "user_client.hpp"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

UserServiceClient::UserServiceClient()
    : _session(Config::get().get_user_service_host(),
               Config::get().get_user_service_port()) {}

UserServiceClient& UserServiceClient::get() {
    static UserServiceClient _instance;
    return _instance;
}

bool UserServiceClient::contains_user(long id) {
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                   Poco::format("/user?id=%li", id),
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    _session.sendRequest(request);
    Poco::Net::HTTPResponse response;
    _session.receiveResponse(response);
    return response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK;
}

// std::optional<long> UserServiceClient::find_user(const std::string&
// first_name,
//                                                  const std::string&
//                                                  last_name) {
//     return std::optional<long>();
// }
