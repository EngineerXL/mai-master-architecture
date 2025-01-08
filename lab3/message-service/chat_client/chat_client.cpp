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

std::optional<database::Chat> ChatServiceClient::get_chat_by_users_array(
    const std::vector<long>& ids) {
    std::stringstream req_ids;
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i) req_ids << ',';
        req_ids << ids[i];
    }
    Poco::Net::HTTPRequest request(
        Poco::Net::HTTPRequest::HTTP_GET,
        Poco::format("/chat/find_by_users?users=%s", req_ids.str()),
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

long ChatServiceClient::create_chat(const database::Chat& chat) {
    std::stringstream req_ids;
    for (size_t i = 0; i < chat.get_users().size(); ++i) {
        if (i) req_ids << ',';
        req_ids << chat.get_users()[i];
    }
    Poco::Net::HTTPRequest request(
        Poco::Net::HTTPRequest::HTTP_POST,
        Poco::format("/chat/?title=%s&users=%s", chat.get_title(),
                     req_ids.str()),
        Poco::Net::HTTPMessage::HTTP_1_1);
    request.setContentType("application/json");
    _session.sendRequest(request);
    Poco::Net::HTTPResponse response;
    std::istream& recv = _session.receiveResponse(response);
    if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
        return database::Chat::fromJSON(
                   {std::istreambuf_iterator<char>(recv), {}})
            .get_id();
    } else {
        throw std::runtime_error("Can't create p2p chat");
    }
}
