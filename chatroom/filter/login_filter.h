#ifndef CHATROOT_FILTER_LOGIN_FILTER_H_
#define CHATROOT_FILTER_LOGIN_FILTER_H_
#include "flare/rpc/http_filter.h"
namespace chatroom {
class LoginFilter : public flare::HttpFilter {
 private:
 public:
  LoginFilter() = default;
  virtual ~LoginFilter() = default;

  Action OnFilter(flare::HttpRequest* request, flare::HttpResponse* response,
                          flare::HttpServerContext* context) override;
};
}  // namespace chatroom

#endif
