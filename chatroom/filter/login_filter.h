#ifndef CHATROOT_FILTER_LOGIN_FILTER_H_
#define CHATROOT_FILTER_LOGIN_FILTER_H_
#include <string_view>

#include "flare/base/function.h"
#include "flare/rpc/builtin/detail/uri_matcher.h"
#include "flare/rpc/http_filter.h"
namespace chatroom {

class LoginFilter : public flare::HttpFilter {
 private:
  flare::detail::UriMatcher _uri_matcher;

 public:
  explicit LoginFilter(flare::detail::UriMatcher uri_matcher = {});
  virtual ~LoginFilter() = default;

  Action OnFilter(flare::HttpRequest* request, flare::HttpResponse* response,
                  flare::HttpServerContext* context) override;
};
}  // namespace chatroom

#endif
