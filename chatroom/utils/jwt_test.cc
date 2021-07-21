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

  auto verifer = jwt::verify()
                   .with_issuer("auth0")
                   .allow_algorithm(jwt::algorithm::hs256{"secret"});
  verifer.verify(decoded, err);
  ASSERT_FALSE(err);
}

TEST(TOKEN_UTIL, test) {
  std::string username = "username";
  auto token = chatroom::TokenUtil::Instance()->GenerateToken(username);
  auto username1 = chatroom::TokenUtil::Instance()->VerifyToken(token);
  ASSERT_TRUE(username1.has_value());
  ASSERT_EQ(username1.value(), username);
}
