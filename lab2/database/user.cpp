#include "../../common/database/user.hpp"

#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>

#include <exception>
#include <sstream>

#include "../config/config.hpp"
#include "../database/database.hpp"

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
}  // namespace database
