#include "chat_handler.hpp"

#include "../../common/user_client/user_client.hpp"

std::vector<long> parse_ids(const std::string &json) {
    std::stringstream ss(json);
    std::vector<long> res;
    for (long elem; ss >> elem;) {
        res.push_back(elem);
        if (ss.peek() == ',') {
            ss.ignore();
        }
    }
    std::sort(res.begin(), res.end());
    return res;
}

void ChatHandler::handleRequest(HTTPServerRequest &request,
                                HTTPServerResponse &response) {
    HTMLForm form(request, request.stream());
    Poco::URI uri(request.getURI());
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    std::cout << request.getMethod() << std::endl;
    try {
        if (uri.getPath() == "/chat" &&
            request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
            // todo generate ids
            database::Chat chat;
            chat.title() = form.get("title");
            chat.users() = parse_ids(form.get("users"));

            bool check_result = true;
            std::string message;
            std::string reason;

            for (long id : chat.users()) {
                if (!UserServiceClient::get().contains_user(id)) {
                    reason = Poco::format("User %li not found", id);
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }
            }

            if (check_result) {
                chat.save_to_database();
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                ostr << chat.get_id();
                return;
            } else {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                std::ostream &ostr = response.send();
                ostr << message;
                response.send();
                return;
            }
        } else if (uri.getPath() == "/chat" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            long id = atol(form.get("id").c_str());

            std::optional<database::Chat> result =
                database::Chat::get_by_id(id);
            if (result) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
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
        } else if (uri.getPath() == "/chat" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT) {
            long id = atol(form.get("id").c_str());

            std::optional<database::Chat> result =
                database::Chat::get_by_id(id);
            if (result) {
                database::Chat chat = result.value();
                chat.title() = form.get("title");
                chat.users() = parse_ids(form.get("users"));

                bool check_result = true;
                std::string message;
                std::string reason;

                for (long id : chat.users()) {
                    if (!UserServiceClient::get().contains_user(id)) {
                        reason = Poco::format("User %li not found", id);
                        check_result = false;
                        message += reason;
                        message += "<br>";
                    }
                }

                if (check_result) {
                    chat.update();
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    std::ostream &ostr = response.send();
                    ostr << chat.get_id();
                    return;
                } else {
                    response.setStatus(
                        Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                    std::ostream &ostr = response.send();
                    ostr << message;
                    response.send();
                    return;
                }
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("status", 404);
                root->set("detail", "User not found");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (uri.getPath() == "/chat" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE) {
            long id = atol(form.get("id").c_str());

            if (database::Chat::remove(id)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                ostr << id;
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