#ifndef PACKETS_CREATE_LOBBY_COMMMAND_H
#define PACKETS_CREATE_LOBBY_COMMMAND_H

# include <packets/Packet.h>

# include <string>
# include <cstdint>

class CreateLobbyCmd : public Packet {
 public:
  ~CreateLobbyCmd() {}
  CreateLobbyCmd() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kCreateLobbyCmd;
  }

# pragma pack(push, 1)
  struct Body {
    char name[kMaxLobbyNameSize] = {0};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_name(const std::string& name) {
    const char *tmp = name.c_str();
    int i = 0;
    while (tmp[i] && i < kMaxLobbyNameSize) {
      cmd_.body.name[i] = tmp[i];
      i++;
    }
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

#endif  // PACKETS_CREATE_LOBBY_COMMMAND_H
