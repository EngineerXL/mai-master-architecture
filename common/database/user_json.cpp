#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

#include "user.hpp"

namespace database {
User User::fromJSON(const std::string &str) {
    User user;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    user.id() = object->getValue<long>("id");
    user.first_name() = object->getValue<std::string>("first_name");
    user.last_name() = object->getValue<std::string>("last_name");
    user.email() = object->getValue<std::string>("email");
    user.login() = object->getValue<std::string>("login");
    user.password() = object->getValue<std::string>("password");
    return user;
}

Poco::JSON::Object::Ptr User::toJSON() const {
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    root->set("id", get_id());
    root->set("first_name", get_first_name());
    root->set("last_name", get_last_name());
    root->set("email", get_email());
    root->set("login", get_login());
    root->set("password", get_password());
    return root;
}

long User::get_id() const { return _id; }

const std::string &User::get_first_name() const { return _first_name; }

const std::string &User::get_last_name() const { return _last_name; }

const std::string &User::get_email() const { return _email; }

const std::string &User::get_login() const { return _login; }

const std::string &User::get_password() const { return _password; }

long &User::id() { return _id; }

std::string &User::first_name() { return _first_name; }

std::string &User::last_name() { return _last_name; }

std::string &User::email() { return _email; }

std::string &User::login() { return _login; }

std::string &User::password() { return _password; }

}  // namespace database