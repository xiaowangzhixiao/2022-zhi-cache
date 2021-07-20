#include "token_util.h"

#include <system_error>

#include "flare/base/string.h"

namespace chatroom {

static const std::string TOKEN_ISSURE = "auth0";
static const std::string TOKEN_TYPE = "JWT";
static const std::string TOKEN_CLAIM_KEY = "NAME";
static const std::string TOKEN_SECRET = "CHATROOM_SECRET";

TokenUtil* TokenUtil::Instance() {
  static flare::NeverDestroyedSingleton<TokenUtil> instance;
  return instance.Get();
}

std::string TokenUtil::GenerateToken(const std::string& username) {
  if (username.empty()) {
    return "";
  }

  return jwt::create()
      .set_issuer(TOKEN_ISSURE)
      .set_type(TOKEN_TYPE)
      .set_payload_claim(TOKEN_CLAIM_KEY, jwt::claim(username))
      .sign(jwt::algorithm::hs256{TOKEN_SECRET});
}

bool TokenUtil::VerifyToken(std::string_view token,
                            const std::string& username) {
  if (username.empty()) {
    return false;
  }
  auto decoded = jwt::decode(std::string(token));
  auto verifier = jwt::verify()
                      .with_issuer(TOKEN_ISSURE)
                      .with_claim(TOKEN_CLAIM_KEY, jwt::claim(username))
                      .allow_algorithm(jwt::algorithm::hs256{TOKEN_SECRET});
  std::error_code err;
  verifier.verify(decoded, err);
  return !err;
}

TokenUtil::TokenUtil() {}

}  // namespace chatroom