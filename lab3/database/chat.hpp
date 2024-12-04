#ifndef CHAT_HPP
#define CHAT_HPP

#include <optional>
#include <string>
#include <vector>

#include "Poco/JSON/Object.h"

namespace database {
class Chat {
   private:
    long _id;
    std::string _title;
    std::vector<long> _users;

   public:
    static Chat fromJSON(const std::string &str);
    Poco::JSON::Object::Ptr toJSON() const;

    void save_to_database();
    static std::optional<Chat> get_by_id(long id);
    void update();
    static bool remove(long id);

    long get_id() const;
    const std::string &get_title() const;
    const std::vector<long> &get_users() const;

    long &id();
    std::string &title();
    std::vector<long> &users();
};
}  // namespace database

#endif /* CHAT_HPP */
