#include "message.hpp"

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

#include <exception>
#include <sstream>

#include "database.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database {

Message Message::fromJSON(const std::string &str) {
    Message chat;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();
    chat.id() = object->getValue<long>("_id");
    chat.chat_id() = object->getValue<long>("_chat_id");
    chat.text() = object->getValue<std::string>("text");
    chat.user() = object->getValue<long>("_user");
    return chat;
}

Poco::JSON::Object::Ptr Message::toJSON() const {
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();
    root->set("_id", _id);
    root->set("chat_id", _chat_id);
    root->set("text", _text);
    root->set("user", _user);
    return root;
}

void Message::save_to_database() {
    database::Database::get().insert("messages", toJSON());
}

std::optional<Message> Message::get_by_id(long id) {
    std::optional<Message> result;
    std::vector<std::string> results =
        database::Database::get().get_by_id("messages", id);
    if (results.size()) result = fromJSON(results[0]);
    return result;
}

std::vector<std::string> Message::get_by_chat_id(long id) {
    Poco::MongoDB::Document selector;
    selector.add("chat_id", id);
    return database::Database::get().select("messages", selector);
}

void Message::update() {
    database::Database::get().update_by_id("messages", _id, toJSON());
}

bool Message::remove(long id) {
    return database::Database::get().remove_by_id("messages", id);
}

long Message::get_id() const { return _id; }

const long &Message::get_chat_id() const { return _chat_id; }

const std::string &Message::get_text() const { return _text; }

const long &Message::get_user() const { return _user; }

long &Message::id() { return _id; }

long &Message::chat_id() { return _chat_id; }

std::string &Message::text() { return _text; }

long &Message::user() { return _user; }
}  // namespace database
