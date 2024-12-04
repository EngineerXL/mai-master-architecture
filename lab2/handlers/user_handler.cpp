#include "user_handler.hpp"

void UserHandler::handleRequest(HTTPServerRequest &request,
                                HTTPServerResponse &response) {
    HTMLForm form(request, request.stream());
    Poco::URI uri(request.getURI());
    response.setChunkedTransferEncoding(true);
    response.setContentType("application/json");
    try {
        if (uri.getPath() == "/user/search/name" &&
            request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            auto results = database ::User::search_by_name(
                form.get("first_name"), form.get("last_name"));
            Poco::JSON::Array arr;
            for (const auto &el : results) arr.add(el.toJSON());
            response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
            std::ostream &ostr = response.send();
            Poco::JSON::Stringifier::stringify(arr, ostr);
            return;
        } else if (uri.getPath() == "/user/search/login" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            std::optional<database::User> result =
                database::User::search_by_login(form.get("login"));
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
                root->set("detail", "User not found");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (uri.getPath() == "/user" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_POST) {
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
        } else if (uri.getPath() == "/user" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_GET) {
            long id = atol(form.get("id").c_str());

            std::optional<database::User> result =
                database::User::get_by_id(id);
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
                root->set("detail", "User not found");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (uri.getPath() == "/user" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_PUT) {
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
                Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
                root->set("status", 404);
                root->set("detail", "User not found");
                root->set("instance", uri.getPath());
                std::ostream &ostr = response.send();
                Poco::JSON::Stringifier::stringify(root, ostr);
                return;
            }
        } else if (uri.getPath() == "/user" &&
                   request.getMethod() == Poco::Net::HTTPRequest::HTTP_DELETE) {
            long id = atol(form.get("id").c_str());

            if (database::User::remove(id)) {
                response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
                std::ostream &ostr = response.send();
                ostr << id;
                return;
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