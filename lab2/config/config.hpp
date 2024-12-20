#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config {
   private:
    Config();
    std::string _host;
    std::string _port;
    std::string _login;
    std::string _password;
    std::string _database;

   public:
    static Config& get();

    std::string& port();
    std::string& host();
    std::string& login();
    std::string& password();
    std::string& database();

    const std::string& get_port() const;
    const std::string& get_host() const;
    const std::string& get_login() const;
    const std::string& get_password() const;
    const std::string& get_database() const;
};

#endif /* CONFIG_HPP */
