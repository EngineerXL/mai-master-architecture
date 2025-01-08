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

std::optional<database::User> UserServiceClient::get_user_by_id(long id) {
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                   Poco::format("/user?id=%li", id),
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    _session.sendRequest(request);
    Poco::Net::HTTPResponse response;
    std::istream& recv = _session.receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        return database::User::fromJSON(
            {std::istreambuf_iterator<char>(recv), {}});
    } else {
        return {};
    }
}
