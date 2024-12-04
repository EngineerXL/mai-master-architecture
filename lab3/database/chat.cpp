#include "chat.hpp"

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

#include <exception>
#include <sstream>

#include "../config/config.hpp"
#include "database.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

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

void Chat::save_to_database() {
    database::Database::get().insert("chats", toJSON());
}

std::optional<Chat> Chat::get_by_id(long id) {
    std::optional<Chat> result;
    std::vector<std::string> results =
        database::Database::get().get_by_id("chats", id);
    if (results.size()) result = fromJSON(results[0]);
    return result;
}

void Chat::update() {
    database::Database::get().update_by_id("chats", _id, toJSON());
}

bool Chat::remove(long id) {
    return database::Database::get().remove_by_id("chats", id);
}

long Chat::get_id() const { return _id; }

const std::string &Chat::get_title() const { return _title; }

const std::vector<long> &Chat::get_users() const { return _users; }

long &Chat::id() { return _id; }

std::string &Chat::title() { return _title; }

std::vector<long> &Chat::users() { return _users; }
}  // namespace database
