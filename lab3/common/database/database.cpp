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

void Database::fill_document(Poco::MongoDB::Document &doc,
                             const Poco::JSON::Object::Ptr &json) {
    for (const auto &[key, value] : *json) {
        if (value.isInteger()) {
            doc.add(key, value.extract<long>());
        } else if (value.isString()) {
            doc.add(key, value.extract<std::string>());
        } else if (value.isArray()) {
            Poco::MongoDB::Array &arr = doc.addNewArray(key);
            Poco::JSON::Array::Ptr ptr =
                value.extract<Poco::JSON::Array::Ptr>();
            for (auto it = ptr->begin(); it != ptr->end(); ++it) {
                arr.add(it->extract<long>());
            }
        } else {
            try {
                Poco::JSON::Object::Ptr child =
                    value.extract<Poco::JSON::Object::Ptr>();
                Poco::MongoDB::Document &child_doc = doc.addNewDocument(key);
                fill_document(child_doc, child);
            } catch (...) {
                doc.add(key, value.toString());
            }
        }
    }
}

void Database::insert(const std::string &collection,
                      const Poco::JSON::Object::Ptr &json) {
    try {
        Poco::SharedPtr<Poco::MongoDB::InsertRequest> insertRequest =
            _database_mongo.createInsertRequest(collection);
        Poco::MongoDB::Document &doc = insertRequest->addNewDocument();
        fill_document(doc, json);
        _connection_mongo.sendRequest(*insertRequest);
    } catch (std::exception &ex) {
        std::cout << "mongodb exception: " << ex.what() << std::endl;
        std::string lastError = _database_mongo.getLastError(_connection_mongo);
        if (!lastError.empty())
            std::cout << "mongodb Last Error: " << lastError << std::endl;
    }
}

std::vector<std::string> Database::select(
    const std::string &collection, const Poco::MongoDB::Document &selector) {
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

void Database::update(const std::string &collection,
                      const Poco::MongoDB::Document &selector,
                      const Poco::JSON::Object::Ptr &json) {
    try {
        Poco::SharedPtr<Poco::MongoDB::UpdateRequest> request =
            _database_mongo.createUpdateRequest(collection);
        request->selector() = selector;
        Poco::MongoDB::Document &doc = request->update();
        fill_document(doc, json);
        _connection_mongo.sendRequest(*request);
    } catch (std::exception &ex) {
        std::cout << "mongodb exception: " << ex.what() << std::endl;
        std::string lastError = _database_mongo.getLastError(_connection_mongo);
        if (!lastError.empty())
            std::cout << "mongodb Last Error: " << lastError << std::endl;
    }
}

bool Database::remove(const std::string &collection,
                      const Poco::MongoDB::Document &selector) {
    std::vector<std::string> result;
    try {
        Poco::SharedPtr<Poco::MongoDB::DeleteRequest> request =
            _database_mongo.createDeleteRequest(collection);
        request->selector() = selector;
        _connection_mongo.sendRequest(*request);
        return true;
    } catch (std::exception &ex) {
        std::cout << "mongodb exception: " << ex.what() << std::endl;
        std::string lastError = _database_mongo.getLastError(_connection_mongo);
        if (!lastError.empty())
            std::cout << "mongodb Last Error: " << lastError << std::endl;
    }
    return false;
}

std::vector<std::string> Database::get_by_id(const std::string &collection,
                                             long id) {
    Poco::MongoDB::Document selector;
    selector.add("_id", id);
    return select(collection, selector);
}

void Database::update_by_id(const std::string &collection, long id,
                            const Poco::JSON::Object::Ptr &json) {
    Poco::MongoDB::Document selector;
    selector.add("_id", id);
    update(collection, selector, json);
}

bool Database::remove_by_id(const std::string &collection, long id) {
    Poco::MongoDB::Document selector;
    selector.add("_id", id);
    return remove(collection, selector);
}

}  // namespace database
