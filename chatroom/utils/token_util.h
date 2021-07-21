#ifndef CHATROOM_UTILS_TOKEN_UTIL_H_
#define CHATROOM_UTILS_TOKEN_UTIL_H_

#include <string>
#include <string_view>
#include <optional>

#include "flare/base/never_destroyed.h"
#include "thirdparty/jwt-cpp/jwt-cpp/jwt.h"

namespace chatroom {

class TokenUtil {
 public:
  static TokenUtil* Instance();

  std::string GenerateToken(const std::string& username);

  std::optional<std::string> VerifyToken(std::string_view token);

 private:
  friend flare::NeverDestroyedSingleton<TokenUtil>;
  TokenUtil();
};

}  // namespace chatroom

#endif  // CHATROOM_UTILS_TOKEN_UTIL_H_