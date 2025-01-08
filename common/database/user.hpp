#ifndef USER_HPP
#define USER_HPP

#include <optional>
#include <string>
#include <vector>

#include "Poco/JSON/Object.h"

namespace database {
class User {
   private:
    long _id;
    std::string _first_name;
    std::string _last_name;
    std::string _email;
    std::string _login;
    std::string _password;

   public:
    static void init();

    static User fromJSON(const std::string &str);
    Poco::JSON::Object::Ptr toJSON() const;

    bool check_name(const std::string &name, std::string &reason);
    bool check_email(const std::string &email, std::string &reason);

    static std::optional<User> get_by_id(long id);
    void save_to_database();
    void update();
    static bool remove(long id);
    static std::vector<User> search_by_name(std::string first_name,
                                            std::string last_name);
    static std::optional<User> search_by_login(std::string first_name);

    long get_id() const;
    const std::string &get_first_name() const;
    const std::string &get_last_name() const;
    const std::string &get_email() const;
    const std::string &get_login() const;
    const std::string &get_password() const;

    long &id();
    std::string &first_name();
    std::string &last_name();
    std::string &email();
    std::string &login();
    std::string &password();
};
}  // namespace database

#endif /* USER_HPP */
