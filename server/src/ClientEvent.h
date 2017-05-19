#ifndef CLIENT_EVENT_H
# define CLIENT_EVENT_H

# include <cstdint>
# include "packets/Packets.h"

class ClientEvent {
 public:
  enum Type {
    kKeysPressed = 0
  };
  // KeysPressed
  struct KeysPressed {
    uint8_t pressed[Packet::kMaxKeys] = {0};
  };

  Type type;
  union {
    KeysPressed keys;
  };

  ClientEvent() {}
  ~ClientEvent() {}
};

#endif  // CLIENT_EVENT_H
