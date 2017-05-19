#ifndef PACKETS_CON_COMMMAND_H
#define PACKETS_CON_COMMMAND_H

# include <packets/Packet.h>

# include <string>
# include <cstdint>

class ConnectCmd : public Packet {
 public:
  ~ConnectCmd() {}
  ConnectCmd() {
    cmd_.header.magic_nb = kMagicNb;
    cmd_.header.code = kConnectCmd;
  }

# pragma pack(push, 1)
  struct Body {
    char name[kMaxClientNameSize] = {0};
  };

  struct Cmd {
    Packet::Header header;
    Body body;
  };
# pragma pack(pop)

  void set_name(const std::string& name) {
    const char *tmp = name.c_str();
    int i = 0;
    while (tmp[i] && i < kMaxClientNameSize) {
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

#endif  // PACKETS_CON_COMMMAND_H
