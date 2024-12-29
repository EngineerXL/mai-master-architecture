#include "message_handler.hpp"

#include "../../common/user_client/user_client.hpp"
#include "../chat_client/chat_client.hpp"

void MessageHandler::handleRequest(HTTPServerRequest &request,
                                   HTTPServerResponse &response) {
    HTMLForm form(request, request.stream());
    Poco::URI uri(request.getURI());
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    std::cout << request.getMethod() << std::endl;
    try {
        if (uri.getPath() == "/message/chat" &&
            request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
            long chat_id = atol(form.get("chat_id").c_str()),
                 user = atol(form.get("user").c_str());

            std::optional<database::Chat> chat =
                ChatServiceClient::get().get_chat(chat_id);
            if (chat.has_value() && chat.value().contains_user(user)) {
                // todo generate ids
                database::Message msg;
                msg.chat_id() = chat_id;
                msg.text() = form.get("text");
                msg.user() = user;
                msg.save_to_database();
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                ostr << msg.get_id();
                return;
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("status", 404);
                root->set("detail",
                          "Chat not found or user is not a member of the chat");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (uri.getPath() == "/message/chat" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            long id = atol(form.get("id").c_str());

            if (ChatServiceClient::get().get_chat(id).has_value()) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(
                    database::Message::get_by_chat_id(id), ostr);
                return;
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("status", 404);
                root->set("detail", "Chat not found");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else {
            response.setStatus(
                Poco::Net::HTTPResponse::HTTPStatus::HTTP_METHOD_NOT_ALLOWED);
            Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
            root->set("status", 405);
            root->set("detail", "Request not allowed");
            root->set("instance", uri.getPath());
            std::ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(root, ostr);
        }
    } catch (Poco::NotFoundException &e) {
        response.setStatus(
            Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST);
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("status", 400);
        root->set("detail", "Request is incomplete");
        root->set("instance", uri.getPath());
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    } catch (...) {
        response.setStatus(
            Poco::Net::HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR);
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("status", 500);
        root->set("detail", "Internal server error");
        root->set("instance", uri.getPath());
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    }
}