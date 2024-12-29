#include "chat_client.hpp"

#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

ChatServiceClient::ChatServiceClient()
    : _session(Config::get().get_chat_service_host(),
               Config::get().get_chat_service_port()) {}

ChatServiceClient& ChatServiceClient::get() {
    static ChatServiceClient _instance;
    return _instance;
}

std::optional<database::Chat> ChatServiceClient::get_chat(long chat_id) {
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                   Poco::format("/chat?id=%li", chat_id),
                                   Poco::Net::HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    _session.sendRequest(request);
    Poco::Net::HTTPResponse response;
    std::istream& recv = _session.receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        return database::Chat::fromJSON(
            {std::istreambuf_iterator<char>(recv), {}});
    } else {
        return {};
    }
}

// std::optional<long> ChatServiceClient::find_user(const std::string&
// first_name,
//                                                  const std::string&
//                                                  last_name) {
//     return std::optional<long>();
// }
