#ifndef CHATROOM_MODEL_MESSAGE_H_
#define CHATROOM_MODEL_MESSAGE_H_
#include <string>

namespace chatroom {

struct Message {
  std::string id;
  std::string text;
};

struct MessageStorage {
  std::string id;
  std::string text;
  std::string timestamp;
};

struct MessageControlData {
  int32_t pageIndex;
  int32_t pageSize;
  MessageControlData() : pageIndex(-1), pageSize(100) {}
};

}  // namespace chatroom
#endif
