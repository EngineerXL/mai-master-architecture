#include "../../common/database/chat.hpp"

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Parser.h>

#include <exception>
#include <sstream>

#include "../../common/database/database.hpp"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database {

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

std::optional<Chat> Chat::get_by_users_array(const std::vector<long> &ids) {
    std::optional<Chat> result;
    std::vector<std::string> results =
        database::Database::get().get_by_users_array("chats", ids);
    if (results.size()) result = fromJSON(results[0]);
    return result;
}

void Chat::update() {
    database::Database::get().update_by_id("chats", _id, toJSON());
}

bool Chat::remove(long id) {
    return database::Database::get().remove_by_id("chats", id);
}
}  // namespace database
