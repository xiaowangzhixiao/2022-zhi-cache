#ifndef CHATROOM_MODEL_USER_H_
#define CHATROOM_MODEL_USER_H_
#include <string>

namespace chatroom {

struct User {
  std::string username;
  std::string firstname;
  std::string lastname;
  std::string email;
  std::string password;
  std::string phone;
};

struct UserResponse {
  std::string firstname;
  std::string lastname;
  std::string email;
  std::string phone;
};

}  // namespace chatroom
#endif
