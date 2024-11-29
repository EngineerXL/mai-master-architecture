#include "database.hpp"

#include "../config/config.hpp"

namespace database {
Database::Database() : _database_mongo(Config::get().get_mongo_database()) {
    std::cout << "Connecting to mongodb: " << Config::get().get_mongo_host()
              << ":" << Config::get().get_mongo_port() << std::endl;
    _connection_mongo.connect(Config::get().get_mongo_host(),
                              std::stoi(Config::get().get_mongo_port()));
}

Poco::MongoDB::Database &Database::get_mongo_database() {
    return _database_mongo;
}

Database &Database::get() {
    static Database _instance;
    return _instance;
}

std::vector<std::string> Database::select(const std::string &collection,
                                          Poco::MongoDB::Document &selector) {
    std::vector<std::string> result;
    try {
        Poco::SharedPtr<Poco::MongoDB::QueryRequest> request =
            _database_mongo.createQueryRequest(collection);
        request->selector() = selector;
        Poco::MongoDB::ResponseMessage response;
        _connection_mongo.sendRequest(*request, response);

        for (const auto &doc : response.documents())
            result.push_back(doc->toString());
    } catch (std::exception &ex) {
        std::cout << "mongodb exception: " << ex.what() << std::endl;
        std::string lastError = _database_mongo.getLastError(_connection_mongo);
        if (!lastError.empty())
            std::cout << "mongodb Last Error: " << lastError << std::endl;
    }
    return result;
}

std::vector<std::string> Database::get_by_id(const std::string &collection,
                                             long id) {
    Poco::MongoDB::Document selector;
    selector.add("_id", id);
    return select(collection, selector);
}

}  // namespace database
