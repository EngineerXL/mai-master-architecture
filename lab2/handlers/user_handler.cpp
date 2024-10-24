#include "user_handler.hpp"

void UserHandler::handleRequest(HTTPServerRequest &request,
                                HTTPServerResponse &response) {
    HTMLForm form(request, request.stream());
    Poco::URI uri(request.getURI());
    try {
        if (request.getURI() == "user/search/name" &&
            request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            auto results = database::User::search_by_name(
                form.get("first_name"), form.get("last_name"));
            Poco::JSON::Array arr;
            for (const auto &el : results) arr.add(el.toJSON());
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            response.setChunkedTransferEncoding(true);
            response.setContentType("application/json");
            std::ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(arr, ostr);
            return;
        } else if (request.getURI() == "user/search/login" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            std::optional<database::User> result =
                database::User::search_by_login(form.get("login"));
            if (result) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                return;
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/not_found");
                root->set("title", "Internal exception");
                root->set("status", "404");
                root->set("detail", "user not found");
                root->set("instance", "/user");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
            database::User user;
            user.first_name() = form.get("first_name");
            user.last_name() = form.get("last_name");
            user.email() = form.get("email");
            user.login() = form.get("login");
            user.password() = form.get("password");

            bool check_result = true;
            std::string message;
            std::string reason;

            if (!user.check_name(user.get_first_name(), reason)) {
                check_result = false;
                message += reason;
                message += "<br>";
            }

            if (!user.check_name(user.get_last_name(), reason)) {
                check_result = false;
                message += reason;
                message += "<br>";
            }

            if (!user.check_email(user.get_email(), reason)) {
                check_result = false;
                message += reason;
                message += "<br>";
            }

            if (check_result) {
                user.save_to_database();
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << user.get_id();
                return;
            } else {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
                std::ostream &ostr = response.send();
                ostr << message;
                response.send();
                return;
            }
        } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            long id = atol(form.get("id").c_str());

            std::optional<database::User> result =
                database::User::get_by_id(id);
            if (result) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(result->toJSON(), ostr);
                return;
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/not_found");
                root->set("title", "Internal exception");
                root->set("status", "404");
                root->set("detail", "user not found");
                root->set("instance", "/user");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT) {
            long id = atol(form.get("id").c_str());

            std::optional<database::User> result =
                database::User::get_by_id(id);
            if (result) {
                database::User user = result.value();
                user.first_name() = form.get("first_name");
                user.last_name() = form.get("last_name");
                user.email() = form.get("email");
                user.login() = form.get("login");
                user.password() = form.get("password");

                bool check_result = true;
                std::string message;
                std::string reason;

                if (!user.check_name(user.get_first_name(), reason)) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (!user.check_name(user.get_last_name(), reason)) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (!user.check_email(user.get_email(), reason)) {
                    check_result = false;
                    message += reason;
                    message += "<br>";
                }

                if (check_result) {
                    user.update();
                    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                    response.setChunkedTransferEncoding(true);
                    response.setContentType("application/json");
                    std::ostream &ostr = response.send();
                    ostr << user.get_id();
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
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/not_found");
                root->set("title", "Internal exception");
                root->set("status", "404");
                root->set("detail", "user not found");
                root->set("instance", "/user");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE) {
            long id = atol(form.get("id").c_str());

            if (database::User::remove(id)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                std::ostream &ostr = response.send();
                ostr << id;
                return;
            } else {
                response.setStatus(
                    Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
                response.setChunkedTransferEncoding(true);
                response.setContentType("application/json");
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("type", "/errors/not_found");
                root->set("title", "Internal exception");
                root->set("status", "404");
                root->set("detail", "user not found");
                root->set("instance", "/user");
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        }
    } catch (Poco::NotFoundException &e) {
        response.setStatus(
            Poco::Net::HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST);
        Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
        root->set("status", static_cast<int>(response.getStatus()));
        root->set("detail", "request is incomplete");
        root->set("instance", uri.getPath());
        std::ostream &ostr = response.send();
        Poco::JSON::Stringifier::stringify(root, ostr);
    } catch (...) {
    }

    response.setStatus(Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    root->set("type", "/errors/not_found");
    root->set("title", "Internal exception");
    root->set("status", Poco::Net::HTTPResponse::HTTPStatus::HTTP_NOT_FOUND);
    root->set("detail", "request ot found");
    root->set("instance", "/user");
    std::ostream &ostr = response.send();
    Poco::JSON::Stringifier::stringify(root, ostr);
}