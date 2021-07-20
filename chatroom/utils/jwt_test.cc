#include "chatroom/utils/token_util.h"

#include <iostream>
#include <system_error>

#include "thirdparty/googletest/gtest/gtest.h"

TEST(JWT, jwt) {
  auto token = jwt::create()
                   .set_issuer("auth0")
                   .set_type("JWT")
                   .set_payload_claim("sample", jwt::claim(std::string("test")))
                   .sign(jwt::algorithm::hs256{"secret"});
  std::cout << token << std::endl;

  auto decoded = jwt::decode(token);
  auto verif = jwt::verify()
                   .with_issuer("auth0")
                   .with_claim("sample", jwt::claim(std::string("test")))
                   .allow_algorithm(jwt::algorithm::hs256{"secre"});
  ASSERT_THROW(verif.verify(decoded), jwt::signature_verification_exception);
  std::error_code err;
  verif.verify(decoded, err);
  ASSERT_TRUE(err);
  ASSERT_EQ(err.category(),
            jwt::error::signature_verification_error_category());
}

TEST(TOKEN_UTIL, test) {
  std::string username = "username";
  auto token = chatroom::TokenUtil::Instance()->GenerateToken(username);
  ASSERT_TRUE(chatroom::TokenUtil::Instance()->VerifyToken(token, username));
}
