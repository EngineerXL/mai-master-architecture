#include <Poco/Net/HTTPClientSession.h>

#include <optional>

#include "../config/config.hpp"

class UserServiceClient {
   private:
    UserServiceClient();

    Poco::Net::HTTPClientSession _session;

   public:
    static UserServiceClient& get();
    bool contains_user(long id);
    std::optional<long> find_user(const std::string& first_name,
                                  const std::string& last_name);
};