#ifndef PACKETS_LEAVE_LOBBY_COMMMAND_H
#define PACKETS_LEAVE_LOBBY_COMMMAND_H

# include <packets/Packet.h>

# include <cstdint>

class LeaveLobbyCmd : public Packet {
 public:
  ~LeaveLobbyCmd() {}
  LeaveLobbyCmd() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kLeaveLobbyCmd;
  }

# pragma pack(push, 1)
  struct Body {
    int32_t lobby_id = 0;
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_id(int32_t lobby_id) {
    cmd_.body.lobby_id = lobby_id;
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

#endif  // PACKETS_LEAVE_LOBBY_COMMMAND_H
