#ifndef CHATROOM_HANDLER_USER_HANDLER_H_
#define CHATROOM_HANDLER_USER_HANDLER_H_

#include "flare/rpc/http_handler.h"

namespace chatroom {
class UserHandler : public flare::HttpHandler {
 public:
  void OnPost(const flare::HttpRequest& request, flare::HttpResponse* response,
              flare::HttpServerContext* context) override;
};

class UserLoginHandler : public flare::HttpHandler {
 public:
  void OnGet(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};

class UserEnterRoomHandler : public flare::HttpHandler {
 public:
  void OnPut(const flare::HttpRequest& request, flare::HttpResponse* response,
             flare::HttpServerContext* context) override;
};
}  // namespace chatroom

#endif  // CHATROOM_HANDLER_USER_HANDLER_H_
