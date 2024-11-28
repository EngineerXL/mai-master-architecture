#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config {
   private:
    Config();
    std::string _mongo_host;
    std::string _mongo_port;
    std::string _mongo_login;
    std::string _mongo_password;
    std::string _mongo_database;

   public:
    static Config& get();

    std::string& mongo_port();
    std::string& mongo_host();
    std::string& mongo_login();
    std::string& mongo_password();
    std::string& mongo_database();

    const std::string& get_mongo_port() const;
    const std::string& get_mongo_host() const;
    const std::string& get_mongo_login() const;
    const std::string& get_mongo_password() const;
    const std::string& get_mongo_database() const;
};

#endif /* CONFIG_HPP */
