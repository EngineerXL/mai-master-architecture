#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/MongoDB/Connection.h>
#include <Poco/MongoDB/Cursor.h>
#include <Poco/MongoDB/Database.h>
#include <Poco/MongoDB/MongoDB.h>

#include <memory>
#include <string>

namespace database {
class Database {
   private:
    Database();

    Poco::MongoDB::Connection _connection_mongo;
    Poco::MongoDB::Database _database_mongo;

   public:
    static Database& get();
    Poco::MongoDB::Database& get_mongo_database();
    std::vector<std::string> select(const std::string& collection,
                                    Poco::MongoDB::Document& selector);
    std::vector<std::string> get_by_id(const std::string& collection, long id);
};
}  // namespace database
#endif /* DATABASE_HPP */
