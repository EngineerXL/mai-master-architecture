#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

#include <exception>
#include <sstream>

#include "chat.hpp"

namespace database {

Chat Chat::fromJSON(const std::string &str) {
    Chat chat;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    chat.id() = object->getValue<long>("_id");
    chat.title() = object->getValue<std::string>("title");
    for (const auto &el : *object->getArray("users")) {
        chat.users().push_back(el.extract<long>());
    }
    return chat;
}

Poco::JSON::Object::Ptr Chat::toJSON() const {
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    root->set("_id", _id);
    root->set("title", _title);
    Poco::JSON::Array::Ptr users = new Poco::JSON::Array();
    for (const auto &user : _users) {
        users->add(user);
    }
    root->set("users", users);
    return root;
}

bool Chat::contains_user(long id) {
    auto it = std::lower_bound(_users.begin(), _users.end(), id);
    return it != _users.end() && *it == id;
}

long Chat::get_id() const { return _id; }

const std::string &Chat::get_title() const { return _title; }

const std::vector<long> &Chat::get_users() const { return _users; }

long &Chat::id() { return _id; }

std::string &Chat::title() { return _title; }

std::vector<long> &Chat::users() { return _users; }
}  // namespace database
