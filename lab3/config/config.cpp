#include "config.hpp"

Config::Config() {
    _mongo_host = std::getenv("MONGO_HOST");
    _mongo_port = std::getenv("MONGO_PORT");
    _mongo_login = std::getenv("MONGO_USER");
    _mongo_password = std::getenv("MONGO_PASSWORD");
    _mongo_database = std::getenv("MONGO_DATABASE");
}

Config &Config::get() {
    static Config _instance;
    return _instance;
}

const std::string &Config::get_mongo_port() const { return _mongo_port; }

const std::string &Config::get_mongo_host() const { return _mongo_host; }

const std::string &Config::get_mongo_login() const { return _mongo_login; }

const std::string &Config::get_mongo_password() const {
    return _mongo_password;
}

const std::string &Config::get_mongo_database() const {
    return _mongo_database;
}

std::string &Config::mongo_port() { return _mongo_port; }

std::string &Config::mongo_host() { return _mongo_host; }

std::string &Config::mongo_login() { return _mongo_login; }

std::string &Config::mongo_password() { return _mongo_password; }

std::string &Config::mongo_database() { return _mongo_database; }
