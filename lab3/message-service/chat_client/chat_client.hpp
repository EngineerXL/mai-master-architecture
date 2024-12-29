#include <Poco/Net/HTTPClientSession.h>

#include <mutex>
#include <optional>

#include "../../common/config/config.hpp"
#include "../../common/database/chat.hpp"

class ChatServiceClient {
   private:
    ChatServiceClient();

    Poco::Net::HTTPClientSession _session;

   public:
    static ChatServiceClient& get();
    std::optional<database::Chat> get_chat(long id);
};