#include "chatroom/filter/login_filter.h"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "flare/base/encoding/base64.h"
#include "flare/base/string.h"
#include "thirdparty/jwt-cpp/jwt-cpp/jwt.h"

using namespace std::literals;

namespace chatroom {

std::optional<std::pair<std::string, std::string>> ParseCredential(
    std::string_view cred) {
  // Authorization:  QWxhZGRpbjpPcGVuU2VzYW1l
  static constexpr auto kPrefix = "Bearer "sv;
  if (!flare::StartsWith(cred, kPrefix)) {
    return std::nullopt;
  }
  return std::nullopt;
}

LoginFilter::LoginFilter(flare::detail::UriMatcher uri_matcher)
    : _uri_matcher(std::move(uri_matcher)) {}

flare::HttpFilter::Action LoginFilter::OnFilter(
    flare::HttpRequest* request, flare::HttpResponse* response,
    flare::HttpServerContext* context) {
  if (!_uri_matcher(request->uri())) {
    return Action::KeepProcessing;
  }

  return LoginFilter::Action::KeepProcessing;
}

}  // namespace chatroom
