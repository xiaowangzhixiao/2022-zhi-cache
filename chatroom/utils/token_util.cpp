#include "token_util.h"

#include <system_error>

#include "flare/base/string.h"

namespace chatroom {

static const std::string TOKEN_ISSURE = "auth0";
static const std::string TOKEN_TYPE = "JWT";
static const std::string TOKEN_CLAIM_KEY = "NAME";

TokenUtil* TokenUtil::Instance() {
  static flare::NeverDestroyedSingleton<TokenUtil> instance;
  return instance.Get();
}

std::string TokenUtil::GenerateToken(const User& user) {
  if (user.username.empty() || user.username.empty()) {
    throw jwt::error::signature_generation_exception();
  }

  return jwt::create()
      .set_issuer(TOKEN_ISSURE)
      .set_type(TOKEN_TYPE)
      .set_payload_claim(TOKEN_CLAIM_KEY, jwt::claim(user.username))
      .sign(jwt::algorithm::hs256{user.password});
}

bool TokenUtil::VerifyToken(std::string_view token, const User& user) {
  if (user.username.empty() || user.username.empty()) {
    throw jwt::error::token_verification_exception();
  }
  auto decoded = jwt::decode(std::string(token));
  auto verifier = jwt::verify()
                      .with_issuer(TOKEN_ISSURE)
                      .with_claim(TOKEN_CLAIM_KEY, jwt::claim(user.username))
                      .allow_algorithm(jwt::algorithm::hs256{user.password});
  std::error_code err;
  verifier.verify(decoded, err);
  return !err;
}

}  // namespace chatroom