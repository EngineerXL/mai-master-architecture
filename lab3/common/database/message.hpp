#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <optional>
#include <string>
#include <vector>

#include "Poco/JSON/Object.h"

namespace database {
class Message {
   private:
    long _id;
    long _chat_id;
    std::string _text;
    long _user;

   public:
    static Message fromJSON(const std::string &str);
    Poco::JSON::Object::Ptr toJSON() const;

    void save_to_database();
    static std::optional<Message> get_by_id(long id);
    void update();
    static bool remove(long id);
    static std::vector<std::string> get_by_chat_id(long id);

    long get_id() const;
    const long &get_chat_id() const;
    const std::string &get_text() const;
    const long &get_user() const;

    long &id();
    long &chat_id();
    std::string &text();
    long &user();
};
}  // namespace database

#endif /* MESSAGE_HPP */
