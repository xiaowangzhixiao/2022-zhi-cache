#ifndef CHATROOM_MODEL_ROOM_H_
#define CHATROOM_MODEL_ROOM_H_
#include <string>
namespace chatroom {

struct Room {
  std::string name;
};

struct RoomStorage {
  std::string id;
  std::string name;
};

struct RoomControlData {
  int32_t pageIndex;
  int32_t pageSize;
  RoomControlData() : pageIndex(0), pageSize(100) {}
};

}  // namespace chatroom
#endif
