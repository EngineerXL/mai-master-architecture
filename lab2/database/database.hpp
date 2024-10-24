#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <Poco/Data/PostgreSQL/Connector.h>
#include <Poco/Data/PostgreSQL/PostgreSQLException.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

#include <memory>
#include <string>

namespace database {
class Database {
   private:
    std::string _connection_string;
    std::unique_ptr<Poco::Data::SessionPool> _pool;
    Database();

   public:
    static Database& get();
    Poco::Data::Session create_session();
};
}  // namespace database
#endif /* DATABASE_HPP */
