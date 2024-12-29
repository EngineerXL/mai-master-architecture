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
    std::string _user_service_host;
    uint16_t _user_service_port;
    std::string _chat_service_host;
    uint16_t _chat_service_port;

   public:
    static Config& get();

    std::string& mongo_port();
    std::string& mongo_host();
    std::string& mongo_login();
    std::string& mongo_password();
    std::string& mongo_database();
    std::string& user_service_host();
    uint16_t& user_service_port();
    std::string& chat_service_host();
    uint16_t& chat_service_port();

    const std::string& get_mongo_port() const;
    const std::string& get_mongo_host() const;
    const std::string& get_mongo_login() const;
    const std::string& get_mongo_password() const;
    const std::string& get_mongo_database() const;
    const std::string& get_user_service_host() const;
    const uint16_t& get_user_service_port() const;
    const std::string& get_chat_service_host() const;
    const uint16_t& get_chat_service_port() const;
};

#endif /* CONFIG_HPP */
