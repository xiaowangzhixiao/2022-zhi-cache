#include "chatroom/filter/login_filter.h"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "chatroom/utils/token_util.h"
#include "flare/base/encoding/base64.h"
#include "flare/base/string.h"
#include "thirdparty/jwt-cpp/jwt-cpp/jwt.h"

using namespace std::literals;

namespace chatroom {

std::optional<std::string_view> ParseCredential(std::string_view cred) {
  // Authorization: 
  static constexpr auto kPrefix = "Bearer "sv;
  if (!flare::StartsWith(cred, kPrefix)) {
    return std::nullopt;
  }
  return cred.substr(kPrefix.size());
}

LoginFilter::LoginFilter(flare::detail::UriMatcher uri_matcher)
    : _uri_matcher(std::move(uri_matcher)) {}

flare::HttpFilter::Action LoginFilter::OnFilter(
    flare::HttpRequest* request, flare::HttpResponse* response,
    flare::HttpServerContext* context) {
  if (_uri_matcher(request->uri())) {
    return Action::KeepProcessing;
  }
  auto token =
      ParseCredential(request->headers()->TryGet("Authorization").value_or(""));
  if (!token) {
    response->set_status(flare::HttpStatus::BadRequest);
    return Action::EarlyReturn;
  }
  auto username = TokenUtil::Instance()->VerifyToken(token.value());
  if (!username) {
    response->set_status(flare::HttpStatus::BadRequest);
    return Action::EarlyReturn;
  }
  request->headers()->Append("username", username.value());
  return LoginFilter::Action::KeepProcessing;
}

}  // namespace chatroom
