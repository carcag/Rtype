#ifndef PACKETS_CLIENT_PACKET_H
#define PACKETS_CLIENT_PACKET_H

# include <packets/Packet.h>

# include <cstdint>

class ClientPack : public Packet {
 public:
  ~ClientPack() {}
  ClientPack() {
  }

# pragma pack(push, 1)
  struct Body {
    int8_t keys[kMaxKeys] = {0};
  };

  struct Cmd {
    Body body;
  };
# pragma pack(pop)

  void set_keys(int8_t keys[kMaxKeys]) {
    for (int i = 0; i < kMaxKeys; ++i)
      cmd_.body.keys[i] = keys[i];
  }

  const char *pack() const {
    char *data = new char[sizeof(Cmd)];
    std::memcpy(data, &cmd_, sizeof(Cmd));
    return data;
  }

  static std::size_t body_size() { return sizeof(Body); }
  static Body unpack(const char *data) {
    Body body;
    std::memcpy(&body, data, sizeof(Body));
    return body;
  }

 private:
  Cmd cmd_;
};

#endif  // PACKETS_CLIENT_PACKET_H
