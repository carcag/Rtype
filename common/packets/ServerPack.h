#ifndef PACKETS_SERVER_PACKET_H
#define PACKETS_SERVER_PACKET_H

# include <packets/Packet.h>

# include <cstdint>

class ServerPack : public Packet {
 public:
  ~ServerPack() {}
  ServerPack() {
  }

# pragma pack(push, 1)
  struct Entity {
    int16_t x = 0;
    int16_t y = 0;
    uint8_t type = 0;
  };

  struct Body {
    uint8_t entities_nb = 0;
    Entity entities[kMaxEntities] = {Entity()};
  };

  struct Cmd {
    Body body;
  };
# pragma pack(pop)

  void add_entity(int16_t x, int16_t y, uint8_t type) {
    int i = cmd_.body.entities_nb;
    cmd_.body.entities[i].x = x;
    cmd_.body.entities[i].y = y;
    cmd_.body.entities[i].type = type;
    cmd_.body.entities_nb++;
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

#endif  // PACKETS_SERVER_PACKET_H
