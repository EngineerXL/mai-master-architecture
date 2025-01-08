#include <Poco/Net/HTTPClientSession.h>

#include <optional>

#include "../config/config.hpp"
#include "../../../common/database/user.hpp"

class UserServiceClient {
   private:
    UserServiceClient();

    Poco::Net::HTTPClientSession _session;

   public:
    static UserServiceClient& get();
    std::optional<database::User> get_user_by_id(long id);
};
