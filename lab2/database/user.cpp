#include "user.hpp"

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

void User::init() {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Statement create_stmt(session);
        create_stmt << "CREATE TABLE IF NOT EXISTS users ("
                    << "id SERIAL PRIMARY KEY,"
                    << "first_name VARCHAR(64) NOT NULL,"
                    << "last_name VARCHAR(64) NOT NULL,"
                    << "email VARCHAR(64),"
                    << "login VARCHAR(64) UNIQUE NOT NULL,"
                    << "password VARCHAR(64) NOT NULL"
                    << ");",
            now;
    }

    catch (Poco::Data::PostgreSQL::PostgreSQLException &e) {
        std::cout << "connection:" << e.displayText() << std::endl;
        throw;
    } catch (Poco::Data::ConnectionFailedException &e) {
        std::cout << "connection:" << e.displayText() << std::endl;
        throw;
    }
}

User User::fromJSON(const std::string &str) {
    User user;
    Poco::JSON::Parser parser;
    Poco::Dynamic::Var result = parser.parse(str);
    Poco::JSON::Object::Ptr object = result.extract<Poco::JSON::Object::Ptr>();

    user.id() = object->getValue<long>("id");
    user.first_name() = object->getValue<std::string>("first_name");
    user.last_name() = object->getValue<std::string>("last_name");
    user.email() = object->getValue<std::string>("email");
    user.login() = object->getValue<std::string>("login");
    user.password() = object->getValue<std::string>("password");

    return user;
}

Poco::JSON::Object::Ptr User::toJSON() const {
    Poco::JSON::Object::Ptr root = new Poco::JSON::Object();

    root->set("id", _id);
    root->set("first_name", _first_name);
    root->set("last_name", _last_name);
    root->set("email", _email);
    root->set("login", _login);
    root->set("password", _password);

    return root;
}

bool User::check_name(const std::string &name, std::string &reason) {
    if (name.length() < 3) {
        reason = "Name must be at least 3 signs";
        return false;
    }

    if (name.find(' ') != std::string::npos) {
        reason = "Name can't contain spaces";
        return false;
    }

    if (name.find('\t') != std::string::npos) {
        reason = "Name can't contain spaces";
        return false;
    }

    return true;
};

bool User::check_email(const std::string &email, std::string &reason) {
    if (email.find('@') == std::string::npos) {
        reason = "Email must contain @";
        return false;
    }

    if (email.find(' ') != std::string::npos) {
        reason = "Email can't contain spaces";
        return false;
    }

    if (email.find('\t') != std::string::npos) {
        reason = "Email can't contain spaces";
        return false;
    }

    return true;
};

std::optional<User> User::get_by_id(long id) {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Poco::Data::Statement select(session);
        User a;
        select << "SELECT id, first_name, last_name, email, login, password "
                  "FROM users WHERE id=$1",
            into(a._id), into(a._first_name), into(a._last_name),
            into(a._email), into(a._login), into(a._password), use(id),
            range(0, 1);

        select.execute();
        Poco::Data::RecordSet rs(select);
        if (rs.moveFirst()) return a;
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
    }
    return {};
}

void User::save_to_database() {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Poco::Data::Statement insert(session);

        insert << "INSERT INTO users "
                  "(first_name,last_name,email,login,password) "
                  "VALUES($1, $2, $3, $4, $5)",
            use(_first_name), use(_last_name), use(_email), use(_login),
            use(_password);

        insert.execute();

        Poco::Data::Statement select(session);
        select << "SELECT LASTVAL()", into(_id), range(0, 1);

        if (!select.done()) {
            select.execute();
        }
        std::cout << "inserted:" << _id << std::endl;
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
        throw;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
        throw;
    }
}

void User::update() {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Poco::Data::Statement update(session);

        update << "UPDATE users SET "
                  "(first_name,last_name,email,login,password) = "
                  "($2, $3, $4, $5, $6) WHERE id=$1",
            use(_id), use(_first_name), use(_last_name), use(_email),
            use(_login), use(_password);

        update.execute();

        std::cout << "updated:" << _id << std::endl;
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
        throw;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
        throw;
    }
}

bool User::remove(long id) {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Poco::Data::Statement remove(session);
        remove << "DELETE FROM users WHERE id=$1", use(id);

        remove.execute();

        if (remove.affectedRowCount()) {
            std::cout << "deleted:" << id << std::endl;
            return true;
        } else {
            return false;
        }
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
    }
    return {};
}

std::vector<User> User::search_by_name(std::string first_name,
                                       std::string last_name) {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Statement select(session);
        std::vector<User> result;
        User a;
        first_name.push_back('%');
        last_name.push_back('%');
        select << "SELECT id, first_name, last_name, email, login, "
                  "password FROM users WHERE first_name LIKE $1 AND last_name "
                  "LIKE $2",
            into(a._id), into(a._first_name), into(a._last_name),
            into(a._email), into(a._login), into(a._password), use(first_name),
            use(last_name), range(0, 1);

        while (!select.done()) {
            if (select.execute()) result.push_back(a);
        }
        return result;
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
        throw;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
        throw;
    }
}

std::optional<User> User::search_by_login(std::string login) {
    try {
        Poco::Data::Session session =
            database::Database::get().create_session();
        Poco::Data::Statement select(session);
        User a;
        login.push_back('%');
        select << "SELECT id, first_name, last_name, email, login, password "
                  "FROM users WHERE login LIKE $1",
            into(a._id), into(a._first_name), into(a._last_name),
            into(a._email), into(a._login), into(a._password), use(login),
            range(0, 1);

        select.execute();
        Poco::Data::RecordSet rs(select);
        if (rs.moveFirst()) return a;
    } catch (Poco::Data::PostgreSQL::ConnectionException &e) {
        std::cout << "connection:" << e.what() << std::endl;
    } catch (Poco::Data::PostgreSQL::StatementException &e) {
        std::cout << "statement:" << e.what() << std::endl;
    }
    return {};
}

const std::string &User::get_login() const { return _login; }

const std::string &User::get_password() const { return _password; }

std::string &User::login() { return _login; }

std::string &User::password() { return _password; }

long User::get_id() const { return _id; }

const std::string &User::get_first_name() const { return _first_name; }

const std::string &User::get_last_name() const { return _last_name; }

const std::string &User::get_email() const { return _email; }

long &User::id() { return _id; }

std::string &User::first_name() { return _first_name; }

std::string &User::last_name() { return _last_name; }

std::string &User::email() { return _email; }
}  // namespace database
